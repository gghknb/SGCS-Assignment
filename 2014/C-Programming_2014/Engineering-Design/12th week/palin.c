#include <stdio.h>

int hanoi(int x,char s,char a,char d);
int step=1;
int main()
{
   int x,testcase,i;
   char s,a,d;
   s='S';
   a='A';
   d='D';
   
   printf("test case:");
   scanf("%d",&testcase);
   for(i=1;i<=testcase;i++)
   {
     printf("How many disks? ");
     scanf("%d",&x);   
     hanoi(x,s,a,d);
     step=1;
   }
   return 0;
}

int hanoi(int x,char s,char a,char d)
{
    if(x==1)
    printf("Step%4d: Move from %c to %c\n",step++,s,d);
    else
    {
       hanoi(x-1,s,d,a);
       hanoi(1,s,a,d);
       hanoi(x-1,a,s,d);
    }
    return 0;
}

