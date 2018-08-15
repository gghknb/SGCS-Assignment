#include <stdio.h>

int Fibonnaci(int);

int main()
{
   int n;
   printf("Input n: ");
   scanf("%d",&n);
   if(n>=0)
     printf("fib(%d): %d\n",n,Fibonnaci(n));
   else
     printf("n cannot be negative number\n");

   return 0;
}

int Fibonnaci(int n)
{
   int i=1;
   int a=0,b=1,c,x; 
   if(n==0||n==1)
       x=n;
    
   else
  { 
     do 
     {   
       x=a+b;
       c=b;
       b=x;
       a=c;  
        i++;
     }   while(i<n);
   }
  return x;
}
