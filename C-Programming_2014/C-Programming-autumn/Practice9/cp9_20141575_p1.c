#include <stdio.h>
#include <stdlib.h>

char* newMyString(int n);
void printMyString(char* my_str);
void fillMyString(char** my_str, char* input);

int main()
{
  char *str;

  str=newMyString(9);
  fillMyString(&str,"CONFUCIUS");
  printMyString(str);
  fillMyString(&str,"Hi");
  printMyString(str);
  fillMyString(&str,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  printMyString(str);

  return 0;
}

char* newMyString(int n)
{
  char *str;
  str=(char *)malloc((n+1)*sizeof(char));

  return str;
}

void printMyString(char* my_str)
{
  int i;

  printf("(%d)",my_str[0]);
  for(i=1;i<=my_str[0];i++)
  {
    printf("%c",*(my_str+i));
  }
  printf("\n");
}
void fillMyString(char** my_str, char* input)
{
  int i;
  int len=0;
  char *str;
  for(i=0;;i++)
  {
    if(*(input+i)=='\0')
      break;
    len++;
  }
  str=(char *)realloc(*my_str,(len+1)*sizeof(char));
  str[0]=len;
  for(i=1;i<=str[0];i++)
  {
    str[i]=*(input+i-1);
  }
}
