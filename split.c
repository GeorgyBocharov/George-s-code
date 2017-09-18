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
  // fixit: отступ = 2 или 4 пробела ... не 1
  // fixit: пробелы вокруг бинарных операторов: (i + 1) * sizeof(char)
 
  // выделение памяти в куче - дорогая операция, если хотите именно в куче получить память, то можно было бы
  // объем памяти увеличивать не на 1, в 2 раза при необходимости ... тогда число realloc'ов будет log2(n), а
  // лишней памяти отъедите максимум в 2 раза больше чем нужно
 s = (char*) realloc(s, (i+1)*sizeof(char));
 i++;
}
s[i]='\0';
while ((tab[j]=getchar())!='\n')
{
 // fixit: у вас цикл while похож на предыдущий. копипаста быть не должно. вынесите в отдельную ф-ю.
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
   // fix it: мы обговарива сигнатуру ф-и, вывод слов нужно вынести из ф-и
    printf("%s\n", point);
    point = strtok (NULL, tab);
  }
}


            

 






















