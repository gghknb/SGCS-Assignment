#include  <stdio.h>

void swap(int *,int *);
void multiple(int,int,int *);

int main()
{
    int a,b,result=0;
    printf("Input first number: ");
    scanf("%d",&a);
    printf("Input second number: ");
    scanf("%d",&b);
    swap(&a,&b);
    if(a<1||b<1)
      printf("Input is less than 1\n");
    else 
    multiple(a,b,&result);
    printf("Result:%d\n",result);
 return 0;
}

void swap(int *a,int *b)
{
   int c;
   if(*a>*b)
   {
      c=*a;
      *a=*b;
      *b=c;
   }
}
  

void multiple(int a,int b,int *result)
{
  
    int sum=0,i;
    for(i=a;i<=b;i++)
    {  if(i%2==0||i%3==0)
       {
        sum+=i;
       }
    }
    *result=sum;
}
     
