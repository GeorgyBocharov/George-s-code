#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char** split (char*, char*, int*);
char* strscan ();
int main ()
{
  char *s, *tab;
  char** head;
  int i = 0, length;
  s = strscan ();
  tab = strscan ();
  head = split ( s, tab, &length);
  for (i = 0; i < length; i++)
  printf ("%s\n", head[i]);  
  free (s);
  free (tab);
  return 0;
}


char** split (char* s ,char* tab, int* length)
{
  int counter = 1, i = 0;
  char** head;
  char* point;
  head = (char**) malloc (sizeof(char*));
  point = strtok (s, tab);
  while (point != NULL)
  {
    if ((i + 1) == counter)
    {
      head = (char**) realloc (head, 2 * counter * sizeof(char*));
      counter = 2 * counter;
    }
    head[i] = point;
    i++;
    counter = counter * 2;
    point = strtok (NULL, tab);
  }
  *length = i;
  return head;
}


char* strscan ()
{
  int i = 0, counter = 1;
  char *s;
  s = (char*) malloc (sizeof(char));
  while ((s[i] = getchar()) != '\n')
  {          
    if ((i + 1) == counter)
    {
      s = (char*) realloc (s, 2 * counter * sizeof(char));
      counter = 2 * counter;
    }
    i++;
  }
  s[i] = '\0';
  return s;
}

 






















