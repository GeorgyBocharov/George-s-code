#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<string.h>

char** split (char*, char*, int*);
char* strscan ();

int main ()
{
  char *s, *tab;
  char** head;
  int i = 0, length;
  printf("enter your string\n");
  s = strscan ();
  printf("enter splitting character\n");
  tab = strscan ();
  head = split ( s, tab, &length);
  for (i = 0; i < length; i++)
  printf ("%s\n", head[i]);
  free (s);
  free (tab);
  free (head);
  return 0;
}


char** split (char* s ,char* tab, int* length)
{
  int counter = 1, i = 0;
  char** head;
  char* point;
  if ( (head = (char**) calloc (1, sizeof(char*))) == NULL)
  {
    printf("head: calloc: %s\n", strerror(errno));
	exit(-1);
  }
  point = strtok (s, tab);
  while (point != NULL)
  {
    if (i + 1 == counter)
    {
      counter *= 2;
	  if ( (head = (char**) realloc (head,  counter * sizeof(char*))) == NULL)
	    {
	      printf("head: realloc: %s\n", strerror(errno));
		  exit(-1);
		}
	}
    head[i] = point;
    i++;
    point = strtok (NULL, tab);
  }
  *length = i;
  return head;
}




char* strscan ()
{
  int i = 0, counter = 1;
  char *s;
  if ( (s = (char*) calloc (1, sizeof(char))) == NULL)
    {
	  printf("strscan: s: calloc: %s\n", strerror(errno));
	  exit(-1);
	}
  while ((s[i] = getchar()) != '\n')
  {          
    if (i + 1 == counter)
    {
      counter *= 2;
	  if ( (s = (char*) realloc (s, counter * sizeof(char))) == NULL)
	  {
	    printf("strscan: s: realloc: %s\n", strerror(errno));
		exit(-1);
	  }
    }
    i++;
  }
  s[i] = '\0';
  return s;
}

 






















