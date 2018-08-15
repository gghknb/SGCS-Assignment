#include <stdio.h>

void swap(int* ,int* );
int absvalue(int );

int main()
{
 int a,b,c;
 printf("Input: "); 
 scanf("%d %d %d",&a,&b,&c);
 
 
  a=absvalue(a);
  b=absvalue(b);
  c=absvalue(c);
  if(a<b)
     swap(&a,&b);
  if(b<c)
     swap(&b,&c);
  if(a<b)
     swap(&a,&b);
  printf("Result: %d %d %d\n",a,b,c);
 
 return 0;
}

void swap(int *a,int *b)
{ 
    int x;
    x=*a;
    *a=*b;
    *b=x;
}  

int absvalue(int x)
{
     if(x<0)
     {
     x=-x;
     return x;
     }
     else return x;
}


