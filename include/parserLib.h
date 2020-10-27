#ifndef PARSERLIB_H
#define PARSERLIB_H
#include <stdio.h>
void parseInt(FILE* file,int* target)
{
  *target = 0;
  char x;
  int found = 0;
  while(1)
  {
	x = fgetc(file);
	//broken or file ends with int
	if (x == EOF)
	{
	  if (found)
		return;
	  *target = 0;
	  return;
	}
	//if # end line
	else if (x == '#')
	{
	  x = fgetc(file);
	  while (x != '\n' && x!=EOF)
	  {
		x = fgetc(file);
	  }
	  	  if (found)
		return;

	  if (x == EOF)
	  {
		*target = 0;
		return;
	  }
	}
	//if digit, add parse
	else if('0' <= x && x <= '9')
	{
	  *target = (*target)*10 + (x - '0');
	  found = 1;
	}
	//else ignore/end
	else
	{
	  if (found)
		return;
	  
	}
  }
}

void parseStr(FILE* file,char**str,int*size)
{
  char cs[256];
  char x;
  int end = 0;
  int found = 0;
  unsigned int i;
  while(1)
  {
	x = fgetc(file);
	//broken
	if (x == EOF)
	{
	  if (found)
	  {
		*str = (char*)malloc(end*sizeof(char));
		*size = end;
		for (i = 0; i < end; ++i)
		  (*str)[i] = cs[i];
		return;
	  }
	  *str = 0;
	  *size = 0;
	  return;
	}
	//if # end line
	else if (x == '#')
	{
	  x = fgetc(file);
	  while (x != '\n' && x!=EOF)
	  {
		x = fgetc(file);
	  }
	  if (found)
	  {
		*str = (char*)malloc(end*sizeof(char));
		*size = end;
		for (i = 0; i < end; ++i)
		  (*str)[i] = cs[i];
		return;
	  }
	  if (x == EOF)
	  {
		*str = 0;
		*size = 0;
		return;
	  }
	}
	//if space, stop
	else if(' ' == x || x == '\n' || x == '\t')
	{
	  if (found)
	  {
		*str = (char*)malloc(end*sizeof(char));
		*size = end;
		for (i = 0; i < end; ++i)
		  (*str)[i] = cs[i];
		return;
	  }
	}
	//else valid char
	else
	{
	  found = 1;
	  cs[end] = x;
	  end++;
	}
  }
}
#endif
