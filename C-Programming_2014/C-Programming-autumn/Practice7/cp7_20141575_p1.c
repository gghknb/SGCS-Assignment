#include <stdio.h>
#include <stdlib.h>

int main()
{
  char *chArr;
  char ch;
  int i,cnt=0,size=1;

  chArr=(char *)malloc(sizeof(char));
  printf("Input String : "); 
  for(i=0;;i++)
  {
    scanf("%c", &ch);
    cnt++;

    if(cnt > size)
    {
      size *= 2;
      chArr=(char *)realloc(chArr,size*sizeof(char));
    }

    *(chArr+i) = ch;
    
    if(ch == '\n')
      break;
  }
  printf("Before : %s",chArr);
  for(i = 0; i < cnt; i++) {
    if(*(chArr+i) >= 'a' && *(chArr+i) <= 'z')
      *(chArr+i)= *(chArr+i)+'A'-'a';
  }
  printf("After : %s", chArr);
  
  free(chArr);
  return 0;
}
  
  

