#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void split (char* ,char* );
int main()
{
  char* s, *tab;
  int i = 0, j = 0;
  s = (char*)malloc(sizeof(char));
  tab = (char*)malloc(sizeof(char));
  while ((s[i] = getchar()) != '\n')
  {
    s = (char*) realloc(s, (i + 1)*sizeof(char));
    i++;
  }
  s[i]='\0';
  while ((tab[j] = getchar()) != '\n')
  {
    tab = (char*) realloc(tab, (j + 1)*sizeof(char));
    j++;
  }
  tab[j] = '\0';
  printf("\n");
  split(s, tab);
  printf("\nstrtok is for deadbeats\n");
  return 0;
}

void split (char* s, char* tab)
{
  int i, k = 0, j, lens, lent, count = 0, begin = 0;
  char* help;
  char** ver;
  help = (char*)malloc(sizeof(char));
  lens = strlen(s);
  lent = strlen(tab);
  for (i = 0;i < lens;i++)
  {
    for (j = 0;j < lent;j++)
    {
      if (s[i] != tab[j])
      help[k] = '1';
      else
      {
         help[k] = '0';
         s[i] = tab[0];
         j = lent;
      }
    }  
    help = (char*)realloc(help, (k + 1)*sizeof(char));
    k++;
  }
  if (help[0] == '1')
  count++;
  for (i = 0;i < k;i++)
  {
    if ((help[i] == '0') && (help[i+1] =='1'))
    count++;
  }
  ver = (char**)malloc(count*sizeof(char*));
  for (i = 0;i < count;i++)
  ver[i] = (char*)malloc(sizeof(char));
  j = 0;
  k = 0;
  i = 0;
  if (s[0] == tab[0])
  {
    while (s[i] == tab[0])
    {
      begin++;
      i++;
    }
  }
  for (i = begin;i < lens;i++)
  {
    if (s[i] != tab[0])
    {
      ver[j][k] = s[i];
      ver[j] = (char*)realloc(ver[j], (k + 1)*sizeof(char));
      k++;
    }
    if ((s[i] == tab[0]) && (s[i + 1] != tab[0]))
    {
      ver[j][k] = '\0';
      j++;
    }
  }
  k = 0;
  j = 0;
  while (j < count)
  {
    while (ver[j][k] != '\0')
    {
      printf("%c", ver[j][k]);
      k++;
    }
    printf("\n");
    j++;
  }  
}







