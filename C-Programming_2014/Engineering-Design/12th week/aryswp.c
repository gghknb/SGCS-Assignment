#include <stdio.h>

void swap(int *,int *);

int main()
{

   int x[10];
   int a,b,i;
   printf("10개의 숫자 입력: ");
   for(i=0;i<10;i++)
   {
     
     scanf("%d",&x[i]);
   }
   printf("몇번째 숫자를 서로 바꾸겠습니까: ");
   scanf("%d %d",&a,&b);
   if(a<0||b<0||a>9||b>9)
     printf("error!\n");
   else
   {
     swap(&x[a],&x[b]);    
     printf("%d %d %d %d %d %d %d %d %d %d\n",x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9]);
   }
  return 0;
}

void swap(int *a,int *b)
{
    int c;
    c=*a;
    *a=*b;
    *b=c;
}
