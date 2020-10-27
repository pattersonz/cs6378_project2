#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <resolv.h>
#include <pthread.h>
#include "unitStructs.h"
#include "vecLib.h"
#include "parseLib.h"

PROC *procs; //array of processes
us totalProcs = 0;//number of processes
us reqNum; //number of requests
us exe; //average execution time
us delay; //average delay time
VEC *eccs; //vector holding return results
pthread_mutex_t procLoc; //make sure two threads can't touch the vector

/*
 * Fills the processes array with the information pulled out of the
 * config.txt
 */
void getProcs();
/*
 * This is a thread function that is tied to a specific process. It
 * will establish a connection, send the process it's relevant info
 * i.e., topology, size of N, port. And it will receive it's
 * results upon finishing.
 */
void *contactProc(void* ptr);

/*
 * Driver
 */
int main()
{
  getProcs();
  fflush(stdout);
  pthread_t *threads;
  threads = (pthread_t*)malloc(totalProcs*sizeof(pthread_t));
  unsigned i;
  for (i = 0; i < totalProcs; ++i)
	pthread_create(&(threads[i]), NULL, &contactProc, (void*)i);
  for (i = 0; i < totalProcs; ++i)
	pthread_join(threads[i],NULL);
  return 0;
}

void *contactProc(void* ptr)
{
  unsigned index = (unsigned)ptr;
  PROC p = procs[index];
  pthread_mutex_lock(&procLoc);
  pthread_mutex_unlock(&procLoc);
  //setup socket connection
  int sock = 0; 
  struct sockaddr_in serv_addr; 
  char buf[1024] = {0};
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  { 
	printf("\n Socket creation error \n"); 
	return (void*)-1; 
  } 

  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(ORIGIN_PORT); 
       
  // Convert IPv4 and IPv6 addresses from text to binary form
  char adr[17] = "xxxx.utdallas.edu";
  unsigned i;
  memcpy(adr,p->mach,4);
  
  char *ip = NULL; 
  struct hostent *adrTrue; 

  // To retrieve host information 
  adrTrue = gethostbyname(adr); 
  checkHostEntry(adrTrue); 
  
  // To convert an Internet network 
  // address into ASCII string
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
  
  //send neighbors
  OMSG o;
  o.id = p->id;
  o.port = p->port;
  o.nCount= totalProcs-1;
  o.neighbors = (char**)malloc(o.nCount*sizeof(char*));
  o.ports = (us*)malloc(o.nCount*sizeof(us));
  o.reqs = reqNum;
  o.exeTime = exe;
  o.delayTime = delay;
  for (i = 0; i < totalProcs; ++i)
  {
	if (i != index)
	{
	  us j;
	  if (i < index)
		j = i;
	  else
		j = i-1;
	  o.neighbors[j] = procs[i].mach;
	  o.ports[j] = procs[i].port;
	}
  }
  serialize_OMSG(buf,o);
  
  send(sock , buf , 1024, 0 );
  free(o.neighbors);
  free(o.ports);
  //wait for results
  int complete = 0;
  while(!complete)
  {
	read( sock , buf, 1024);
	complete = true;
  }
  //print results
  return (void*)0; 
}

void getProcs()
{
  //read file
  FILE * file;
  file = fopen("config.txt", "r");
  int numProcs,avgDelay,avgExe,requests;
  unsigned int i;
  parseInt(file,&numProcs);
  parseInt(file,&avgDelay);
  parseInt(file,&avgExe);
  parseInt(file,&requests);
  totalProcs = numProcs;
  delay = avgDelay;
  exe = avgExe;
  reqNum = requests;
  procs = (PROC*) malloc(numProcs*sizeof(PROC));
  for (i = 0; i < numProcs; ++i)
  {
	int id;
	char* machine;
	int size;
	int port;
	parseInt(file,&id);
	parseStr(file,&machine,&size);
	parseInt(file,&port);
	procs[i].id = id;
	procs[i].mach = machine;
	procs[i].port = port;
  }
  fclose(file);
  return;
}

  
