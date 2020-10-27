#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <resolv.h>
#include <pthread.h>
#include "vecLib.h"
#include "unitStructs.h"

#define MAX_(x,y) ((x) > (y) ? (x) : (y))

us thisId; //the id associated with this process.
us nCount; //number of neighbors
us thisPort; //port this process is listening on
NEIGHBOR* ns; //array of neighbors
us foundNum; //number of found processes
us curRound; //current round of the processes
us thisClock; //current clock value
us delay; //average delay time
us exe; //average execution time
us reqClock; //clock at time of request
us attemptingEntry; //flag for identifying whether there is currently an attempt at the critical section

us responses; //the number of responses expected
pthread_mutex_t resLock; //lock for editing number of responses
pthread_mutex_t clockLock; //lock for editing clock
pthread_barrier_t syncer; //barrier to sync threads
pthread_barrier_t gotRes; //barrier for found all responses
pthread_barrier_t csDone; //barrier for found all responses

VEC vec; //vector of found nodes (id and round)
pthread_mutex_t vecLock; //lock for editing vector of nodes

/*
 * Thread funciton to contact the origin. Opens on start and
 * listens on the designated originPort for information.
 */
void *contactOrigin(void* ptr);

/*
 * opens up sockets to accept requests for communication from
 * neighbors. Can accomodate as many messages as needed. Each
 * new accpet opens an instance of handleMsg
 */
void *receiver(void* ptr);

/*
 * Once the port has been accepted, the handler gets what round
 * the contacting processes is on and returns what ids it was able
 * to find the previous round. 
 */
void *handleMsg(void* ptr);

/*
 * Attempts to open a connection with a listening neighbor, upon connection
 * a loop is executed until N ids were found. Each round all neighbors send
 * potential candidates. If a candidate is not in the found list, it is added.
 * (this is all done in mutex)
 */
void *requester(void* ptr);

/*
 * Calls for an enter to the critical section, only enters after all
 * all neighbors have confirmed the request. Holds off on responding
 * to other processes with less priority
 */
void csEnter();

/*
 * Responds to all awaiting processes
 */
void csLeave();

/*
 * delays a random amount of time centered at x
 */
void randDelay(us x);

/*
 * Driver
 */
int main()
{
  attemptingEntry = 0;
  pthread_t originThread;
  pthread_create(&originThread, NULL, &contactOrigin, NULL);
  pthread_join(originThread, NULL);
  return -1;
} 

void *contactOrigin(void* ptr)
{
  int server_fd, new_socket, valread; 
  struct sockaddr_in address; 
  int opt = 1; 
  int addrlen = sizeof(address); 
  byte buf[1024] = {0}; 
      
  // Creating socket file descriptor 
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
  { 
	perror("socket failed"); 
	return (void*)-1; 
  } 
       
  // Forcefully attaching socket to the port 8080 
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
				 &opt, sizeof(opt))) 
  { 
	perror("setsockopt"); 
	return (void*)-1; 
  } 
  address.sin_family = AF_INET; 
  address.sin_addr.s_addr = INADDR_ANY; 
  address.sin_port = htons( ORIGIN_PORT ); 
       
  // Forcefully attaching socket to the port 8080 
  if (bind(server_fd, (struct sockaddr *)&address,  
		   sizeof(address))<0) 
  { 
	perror("bind failed"); 
	return (void*)-1;
  } 
  if (listen(server_fd, 3) < 0) 
  { 
	perror("listen"); 
    return (void*)-1; 
  } 
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
						   (socklen_t*)&addrlen))<0) 
  { 
	perror("accept"); 
    return (void*)-1; 
  }
  
  valread = read( new_socket , buf, 1024);
  OMSG o;
  deserialize_OMSG(buf,&o);
  printOMSG(o);
  nCount = o.nCount;
  ns = (NEIGHBOR*)malloc(nCount*sizeof(NEIGHBOR));
  delay = o.delayTime;
  exe = o.exeTime;
  thisClock = 0;
  us reqs = o.reqs;
  unsigned i;
  for (i = 0; i < nCount; ++i)
  {
	ns[i].port = o.ports[i];
	ns[i].name = o.neighbors[i];
  }
  thisPort = o.port;
  thisId = o.id;
  
  //find eccentricity
  //set up nCount senders and a reciever
  pthread_t recT;
  pthread_create(&recT, NULL, &receiver, NULL);

  pthread_t *threads;
  threads = (pthread_t*)malloc(nCount*sizeof(pthread_t));
  responses = nCount;
  pthread_barrier_init(&syncer, NULL, nCount+1);
  pthread_barrier_init(&gotRes, NULL, nCount+1);
  pthread_barrier_init(&csDone, NULL, nCount+1); 
  
  for (i = 0; i < nCount; ++i)
	pthread_create(&(threads[i]), NULL, &requester, (void*)&ns[i]);

  while (reqs)
  {
	attemptingEntry = 1;
	byte * msg = buf;
  	randDelay(delay);
	attemptingEntry = 1;
	msg = serialize_char_ptr(msg,"Requesting",10);
	pthread_mutex_lock(&clockLock);
	msg = serialize_u_short(msg,thisClock);
	reqClock = thisClock;
	pthread_mutex_unlock(&clockLock);
	msg = serialize_u_short(msg,thisId);
	send(new_socket , buf , 1024, 0 );
	csEnter();
	msg = buf;
	msg = serialize_char_ptr(msg,"Entering",8);
	pthread_mutex_lock(&clockLock);
	msg = serialize_u_short(msg,thisClock);
	pthread_mutex_unlock(&clockLock);
	msg = serialize_u_short(msg,thisId);
	send(new_socket , buf , 1024, 0 );
	randDelay(exe);
	csLeave();
	msg = buf;
	msg = serialize_char_ptr(msg,"Exiting",7);
	pthread_mutex_lock(&clockLock);
	msg = serialize_u_short(msg,thisClock);
	pthread_mutex_unlock(&clockLock);
	msg = serialize_u_short(msg,thisId);
	send(new_socket , buf , 1024, 0 );
	reqs--;
	attemptingEntry = 0;
  }
  
  for (i = 0; i < nCount; ++i)
	pthread_join(threads[i],NULL);
  printf("Complete:%d\n",curRound);
  fflush(stdout);
  byte * msg = buf;
  msg = serialize_char_ptr(msg,"Complete",8);
  pthread_mutex_lock(&clockLock);
  msg = serialize_u_short(msg,thisClock);
  pthread_mutex_unlock(&clockLock);
  msg = serialize_u_short(msg,thisId);
  send(new_socket , buf , 1024, 0 );
  pthread_join(recT, NULL);
  return 0; 
}

void *requester(void *ptr)
{
  NEIGHBOR* n = (NEIGHBOR*)ptr;
  int sock = 0;
  struct sockaddr_in serv_addr;
  byte buf[1024] = {0};
  char *ip = NULL;
  struct hostent *adrTrue;
  char adr[17] = "xxxx.utdallas.edu";
  unsigned i;
  while (1)
  {
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	  printf("\n Socket creation error \n");
	  return (void*)-1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(n->port);
	memcpy(adr,n->name,4);
 
	adrTrue = NULL;
	adrTrue = gethostbyname(adr);
	if (adrTrue != NULL && adrTrue->h_addr_list[0] != NULL)
	  break;
  }
  while(ip == NULL)
    ip = inet_ntoa(*((struct in_addr*)
                     adrTrue->h_addr_list[0]));

  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
  {
	printf("\nInvalid address \"%s\" Address not supported \n",ip);
	return (void*)-1;
  }

  int ress = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  while (ress < 0)
  {
	printf("\nConnection Failed retrying\n");
	ress = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  }
  while (1)
  {
	//make sure all procs are ready
	pthread_barrier_wait(&syncer);

	serialize_u_short(buf,curRound);
	send(sock , buf , 1024, 0 );

	pthread_mutex_lock(&clockLock);
	thisClock++;
	pthread_mutex_unlock(&clockLock);
	
	int readTag = read(sock, buf, 1024);
	if (readTag <= 0)
	  break;
//wait for all responses to be received
	pthread_barrier_wait(&gotRes);
	pthread_barrier_wait(&csDone);
  }
  close(sock);
  return (void*)0;
}

void *receiver(void* ptr)
{
  int server_fd, new_socket, valread; 
  struct sockaddr_in address; 
  int opt = 1; 
  int addrlen = sizeof(address); 
  byte buf[1024] = {0}; 
      
  // Creating socket file descriptor 
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
  { 
	perror("socket failed"); 
	return (void*)-1; 
  } 
       
  // Forcefully attaching socket to the port 8080 
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
				 &opt, sizeof(opt))) 
  { 
	perror("setsockopt"); 
	return (void*)-1; 
  } 
  address.sin_family = AF_INET; 
  address.sin_addr.s_addr = INADDR_ANY; 
  address.sin_port = htons( thisPort ); 
       
  if (bind(server_fd, (struct sockaddr *)&address,  
		   sizeof(address))<0) 
  { 
	perror("bind failed"); 
	return (void*)-1;
  } 
  if (listen(server_fd, 5) < 0) 
  { 
	perror("listen"); 
    return (void*)-1; 
  }
  VEC vt;
  init(&vt);
  while (1)
  {
	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen); 
	if (new_socket < 0)
	{ 
	  perror("new socket error"); 
	  return (void*)-1; 
	}
	THREADDATA* td = (THREADDATA*)malloc(sizeof(THREADDATA));
	td->socket = new_socket;
	pthread_create(&(td->data), NULL, &handleMsg, (void*)&(td->socket));
  	
	putBack(&vt,td);
  }
  us k;
  for (k = 0; k < vt.size;++k)
  {
	THREADDATA* td;
	td = (THREADDATA*)get(&vt,k);
	pthread_join(td->data, NULL);
	free(td);
  }
  clear(&vt);
}

void *handleMsg(void* ptr)
{
  while(1)
  {
	int sock = *((int *) ptr);
	byte buf[1024];
	int readTag;
	readTag = read(sock, buf, 1024);
	if (readTag <= 0)
	  break;
	UMSG u;
	deserialize_UMSG(buf,&u);
	byte* msg = buf;
	msg = serialize_u_short(msg,thisClock);
	pthread_mutex_lock(&clockLock);
	thisClock = MAX_(thisClock + 1, u.clock);
	pthread_mutex_unlock(&clockLock);
	
	//if valid respond, else hold until CS section hit
	if (attemptingEntry)
	{
	  if ( (u.clock > reqClock) ||
		   (u.clock == reqClock && u.id > thisId))
		{
		  while (attemptingEntry)
			;
		}
	}
	send(sock , buf , 1024, 0 );
  }
}

void csEnter()
{
  //make sure all procs are ready
  pthread_barrier_wait(&syncer);
  //wait for all responses to be received
  pthread_barrier_wait(&gotRes);
}

void csLeave()
{
  pthread_barrier_wait(&csDone);
}

void randDelay(us x)
{
}
