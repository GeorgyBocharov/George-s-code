#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void Generatestring (int n, char* str);
int main()
{
 // обычно отсутуп делают в 2 или 4 пробела ... 1 мало
 int i, p, k;
 char* s;
 scanf("%d", &p);
 k = (1 << p) - 1;
 s = (char*)malloc(sizeof(char)*k);
 // пробелы вокруг *, + и т.п.
 Generatestring (p, s);
 printf ("%s", s);
 // очистить память
}
void Generatestring (int n, char* str)
{
 int m=0 ,k;
 for (k = 1; k <= n; k++)
 {
  strncpy(&str[m+1], str, sizeof(char) * m);
  str[m] = 'a' + k - 1;
  m = (1 << k) - 1;
 }
}
 
