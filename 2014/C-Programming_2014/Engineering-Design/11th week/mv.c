#include <stdio.h>

int main()
{
   int n,i=0,sum=0,x=0;
   float m,v;
  while(1)
  {  
   printf("Input %dth number: ",i);
   scanf("%d",&n);
   
   if(n<0)
    break;
   else if(n<20||n>50)
    continue;
   else if(n>=20&&n<=50)
    {
       sum+=n;
       x+=n*n;  
       i++;
       
         
   }
  }
  
   m=(float)sum/(float)i;
   v=(float)x/i-m*m;
   printf("Mean: %0.3f\n",m);
   printf("Var: %0.3f\n",v);
   return 0;
}
