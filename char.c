#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// GenerateString ... каждое слово с большой
// либо generateString ... первую букву можно строчной оставить
void Generatestring (int n, char* str);
int main()
{
  int i, p, k;
  char* s;
  scanf("%d", &p);
  k = (1 << p) - 1;
  s = (char*)malloc(sizeof(char) * k);
  Generatestring (p, s);
  printf ("%s", s);
  free (s);
  return 0;
}
void Generatestring (int n, char* str)
{
  int m = 0, k;
  for (k = 1; k <= n; k++)
  {
    strncpy(&str[m + 1], str, sizeof(char) * m);
    str[m] = 'a' + k - 1;
    m = (1 << k) - 1;
  }
}
 
