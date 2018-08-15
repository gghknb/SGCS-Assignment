#include <stdio.h>

int main()
{
   int x;
   int a=1, b=0;

   printf("Input: ");
   scanf("%d",&x);
   while(a<x+1)
   {
     while(b<x-a)
     {
        printf(" ");
        b++;
     }
     b=0;
     while(b<2*a-1)
     {
       printf("*");
       b++;
     }
     b=0;
     printf("\n");
     a++;
   }
   return 0;
}
