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
  // fixit: память под head не очистили ... в целом, конечно, не здорово чистить память не там, где её выделяли
  // можно было выделить память под head в main, оценив сверху число слово, как половина количества букв в сточке + 1
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
    counter = counter * 2; // выглядит как ошибка ... непонятно зачем после каждого слова увеличивать counter вдвое
    point = strtok (NULL, tab);
  }
  *length = i;
  return head;
}

// вас fgets не устроил только тем, что нужно заранее выделить память под строку, а совсем с запасом не хочется выделять?
// ну, ок ...
char* strscan ()
{
  int i = 0, counter = 1;
  char *s;
  s = (char*) malloc (sizeof(char));
  while ((s[i] = getchar()) != '\n')
  {          
    // лишние скобки -> (i + 1 == counter)
    if ((i + 1) == counter)
    {
      s = (char*) realloc (s, 2 * counter * sizeof(char));
      counter = 2 * counter;
      // так чуть короче:
      // counter *= 2;
      // s = (char*) realloc (s, counter * sizeof(char));
    }
    i++;
  }
  s[i] = '\0';
  return s;
}

 






















