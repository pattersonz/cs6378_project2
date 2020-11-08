#ifndef UNITSTRUCTS_H
#define UNITSTRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "vecLib.h"
#define ORIGIN_PORT 1024
typedef unsigned char byte; //shorthand
typedef unsigned short us; //shorthand
//All necessary information to establish a process
typedef struct proc
{
  us id;
  char* mach;
  us port;
} PROC;
//information to be sent from origin to unit
typedef struct origin_message
{
  us id;
  us port;
  us nCount;
  char** neighbors;
  us *ports;
  us reqs;
  us exeTime;
  us delayTime;
} OMSG;
//information to be sent from unit to unit
typedef struct unit_message
{
  us request;
  us clock;
  us id;
} UMSG;
//collection for contacting a neighbor
typedef struct neighbor_pair
{
  us port;
  char *name;
} NEIGHBOR;
//collection for results
typedef struct eccen
{
  us id;
  us round;
} ECC;
//collection for threads
typedef struct pthreadData
{
  pthread_t data;
  int socket;
} THREADDATA;

/*
 * comparison function for searching for ECCS
 */
int EccIDEq(void* a, void* b)
{
  return ((ECC*)(b))->id == *((unsigned short*)a);
}

//prints a process
void printProcs(unsigned count, PROC* p)
{
  unsigned i,j;
  for (i = 0; i < count; ++i)
  {
	printf("proc:%d\n\tmachine:%s\n\tport:%d\n\tNeighbors:",p[i].id,p[i].mach,p[i].port);
	printf("\n");
  }
}

//print an origin message
void printOMSG(OMSG o)
{
  printf("id:%d port:%d neighbors:%d\n", o.id, o.port, o.nCount);
  unsigned i;
  for (i = 0; i < o.nCount; ++i)
  {
	printf("\tn:%d name:%s port:%d\n", i, o.neighbors[i], o.ports[i]);
  }
  fflush(stdout);
}

/*
 * The following is all serializing and deserializing, or converting
 * to and from byte arrays. All serializing is done in a manner so that
 * however the data is transfered, it comes out in the same way.
 */

byte *serialize_u_short(byte *buf, us v)
{
  buf[0] = v >> 8;
  buf[1] = v;
  fflush(stdout);
  return buf + 2;
}

byte *serialize_u_short_ptr(byte *buf, us *v, us size)
{
  buf = serialize_u_short(buf,size);
  unsigned i;
  for (i = 0; i < size; ++i)
	buf = serialize_u_short(buf,v[i]);
  return buf;
}

byte *serialize_char(byte *buf, char c)
{
  buf[0] = c;
  return buf + 1;
}

byte *serialize_char_ptr(byte* buf, char* c, us size)
{
  buf = serialize_u_short(buf,size);
  unsigned i;
  for (i = 0; i < size; ++i)
	buf = serialize_char(buf,c[i]);
  return buf;
}

byte *serialize_char_ptr2(byte* buf, char** c, us *size, us size2)
{
  buf = serialize_u_short(buf,size2);
  unsigned i;
  for (i = 0; i < size2; ++i)
	buf = serialize_char_ptr(buf,c[i],size[i]);
  return buf;
}

byte *serialize_OMSG(byte* buf, OMSG o)
{
  //us id;
  buf = serialize_u_short(buf,o.id);
  //us port;
  buf = serialize_u_short(buf,o.port);
  //us nCount;
  buf = serialize_u_short(buf,o.nCount);
  //char** neighbors;
  us *temp = (us*)malloc(o.nCount*sizeof(us));
  unsigned i;
  for (i = 0; i < o.nCount; ++i)
	temp[i] = 4;
  buf = serialize_char_ptr2(buf,o.neighbors,temp,o.nCount);
  free(temp);
  //us *ports;
  buf = serialize_u_short_ptr(buf,o.ports,o.nCount);
  buf = serialize_u_short(buf,o.reqs);
  buf = serialize_u_short(buf,o.exeTime);
  buf = serialize_u_short(buf,o.delayTime);
  return buf;
}

byte *serialize_UMSG(byte* buf, UMSG u)
{
  //us request
  buf = serialize_u_short(buf,u.request);
  //us count
  buf = serialize_u_short(buf,u.clock); 
  //us *ids;
  buf = serialize_u_short(buf,u.id);
  return buf;
}

byte *serialize_VEC_ECC(byte* buf, VEC *v)
{
  buf = serialize_u_short(buf, v->size);
  VECDATA *temp = v->container;
  while(temp != NULL)
  {
	buf = serialize_u_short(buf, ((ECC*)(temp->data))->id);
	buf = serialize_u_short(buf, ((ECC*)(temp->data))->round);
	temp = temp->next;
  }
  return buf;
}

byte *deserialize_u_short(byte *buf, us* v)
{
  (*v) = buf[0];
  (*v) = (*v) << 8;
  (*v) = (*v) + buf[1];
  fflush(stdout);
  return buf + 2;
}

byte *deserialize_u_short_ptr(byte *buf, us **v)
{
  us s;
  buf = deserialize_u_short(buf,&s);
  unsigned i;
  (*v) = (us*)malloc(s*sizeof(us));
  for (i = 0; i < s; ++i)
	buf = deserialize_u_short(buf,&((*v)[i]));
  return buf;
}

byte *deserialize_char(byte *buf, char *c)
{
  *c = buf[0];
  return buf + 1;
}

byte *deserialize_char_ptr(byte* buf, char** c)
{
  us s;
  buf = deserialize_u_short(buf,&s);
  (*c) = (char*)malloc(s*sizeof(char));
  unsigned i;
  for (i = 0; i < s; ++i)
	buf = deserialize_char(buf,&((*c)[i]));
  return buf;
}

byte *deserialize_char_ptr_u(byte* buf, char** c, us*s)
{
  buf = deserialize_u_short(buf,s);
  (*c) = (char*)malloc(*s*sizeof(char));
  unsigned i;
  for (i = 0; i < *s; ++i)
	buf = deserialize_char(buf,&((*c)[i]));
  return buf;
}

byte *deserialize_char_ptr2(byte* buf, char*** c)
{
  us s;
  buf = deserialize_u_short(buf,&s);
  (*c) = (char**)malloc(s*sizeof(char*));
  unsigned i;
  for (i = 0; i < s; ++i)
	buf = deserialize_char_ptr(buf,&((*c)[i]));
  return buf;
}

byte *deserialize_OMSG(byte* buf, OMSG *o)
{
  //us id;
  buf = deserialize_u_short(buf,&(o->id));
  //us port;
  buf = deserialize_u_short(buf,&(o->port));
  //us nCount;
  buf = deserialize_u_short(buf,&(o->nCount));
  //char** neighbors;
  buf = deserialize_char_ptr2(buf,&(o->neighbors));
  //us *ports;
  buf = deserialize_u_short_ptr(buf,&(o->ports));
  //us reqs
  buf = deserialize_u_short(buf,&(o->reqs));
  //us exeTime
  buf = deserialize_u_short(buf,&(o->exeTime));
  //us delayTime
  buf = deserialize_u_short(buf,&(o->delayTime));

  return buf;
}

byte *deserialize_UMSG(byte* buf, UMSG *u)
{
  //us request
  buf = deserialize_u_short(buf,&(u->request));
  //us count
  buf = deserialize_u_short(buf,&(u->clock)); 
  //us *ids;
  buf = deserialize_u_short(buf,&(u->id));
  return buf;
}

byte *deserialize_VEC_ECC(byte* buf, VEC *v)
{
  us s;
  buf = deserialize_u_short(buf, &s);
  if (s == 0)
	return buf;
  init(v);
  us i;
  for (i = 0; i < s; ++i)
  {
	ECC* e = (ECC*)malloc(sizeof(ECC));
	buf = deserialize_u_short(buf, &e->id);
	buf = deserialize_u_short(buf, &e->round);
	putBack(v,(void*)e);
  }
  return buf;
}


/*
 * returns the number of instances in a vector
 * which have the specified round number
 */
void roundCount(VEC* vec, us r, us*c)
{
  *c = 0;
  us i;
  for (i = 0; i < vec->size; ++i)
	if (((ECC*)get(vec,i))->round == r)
	  ++(*c);
}

/*
 * fills an array of ids with all found on the 
 * specified round
 */
void fillWithRound(VEC* v, us* ids, us r)
{
  us c = 0,i;
  for (i = 0; i < v->size; ++i)
	if (((ECC*)get(v,i))->round == r)
	{
	  ids[c] = ((ECC*)get(v,i))->id;
	  ++c;
	}
}

//prints ECC vector
void printECC(VEC* v)
{
  us max = 0,i,j;
  for(i = 0; i < v->size; ++i)
	if (((ECC*)(get(v,i)))->round > max)
	  max = ((ECC*)(get(v,i)))->round;
  for (i = 0; i <= max; ++i)
  {
	printf("\tR:%d\n\t\t",i);
	for(j = 0; j < v->size; ++j)
	  if (((ECC*)(get(v,j)))->round == i)
		printf("%d ",((ECC*)(get(v,j)))->id);
	printf("\n");
  }
}

// Returns host information corresponding to host name 
void checkHostEntry(struct hostent * hostentry) 
{ 
    if (hostentry == NULL) 
    { 
        perror("gethostbyname"); 
        exit(1); 
    } 
} 
#endif
