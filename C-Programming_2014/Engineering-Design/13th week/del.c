#include <stdio.h>

int main()
{
   char x[21];
   int k,a,i;
   printf("20글자 이내의 문자 입력: ");
   for(i=0;i<20;i++)
    { 
     scanf("%c",&x[i]);
    if(x[i]=='\n')
      break;
    }
    k=i;
    printf("삭제할 문자 위치 입력: ");
    scanf("%d",&a);
    for(i=0;i<20-a;i++)
      x[a+i]=x[a+i+1];
  
  for(i=0;i<k;i++)
      printf("%c",x[i]);

    return 0;
}
