#include <stdio.h>
#include <stdlib.h>

void printByWord(char* str);

int main()
{
  char *str;
  int i;
  str=(char *)malloc(sizeof(char)*40);
  printf("Input string : ");
  for(i=0;i<40;i++)
  {
    scanf("%c",str+i);
    if(*(str+i)=='\n')
      break;
  }
  
  printByWord(str);  

  free(str);
  return 0;
}

void printByWord(char* str)
{
  int i;  
  printf("- Result -\n");
  printf("\t ");
  if((*(str+0)>='a'&&*(str+0)<='z')||*(str+0)>='A'&&*(str+0)<='Z')
  {
    for(i=0;i<39;i++)
    {
      if(*(str+i)=='\n')
        break;
      if(*(str+i)==' ')
      {
        printf("\n");
        printf("\t");
      }
      printf("%c",*(str+i));
    }
    printf("\n");
  }
  else
  {
    printf("%40.20s\n",str);
  }
}
