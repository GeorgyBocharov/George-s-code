#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void split (char*  ,char* );
int main()
{
char* s,*tab;
int i=0,j=0;
s = (char*)malloc(sizeof(char));
tab = (char*)malloc(sizeof(char));
while ((s[i]=getchar())!='\n')
{
 s = (char*) realloc(s, (i+1)*sizeof(char));
 i++;
}
s[i]='\0';
while ((tab[j]=getchar())!='\n')
{
 tab = (char*) realloc(tab, (j+1)*sizeof(char));
 j++;
}
tab[j]='\0';
split( s, tab);
return 0;
}



void split (char* s ,char* tab)
{
char* point;
  point = strtok(s, tab);
  while (point != NULL)
  {
    printf("%s\n", point);
    point = strtok (NULL, tab);
  }
}


            

 






















