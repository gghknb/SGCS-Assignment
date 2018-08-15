#include <stdio.h>

int main()
{
   int n,m,count=0;
   int a=0;
   int b,c,sum;
   float avg;
   printf("Number of class: ");
   scanf("%d",&n);
   printf("Outline : ");
   scanf("%d",&m);

   while(a<n)
   {
      printf("Input socre #%d : ",a);
      scanf("%d",&b);
      sum+=b;
      a++;
      
      if(m>b)
      count++;
      
       
   }
   avg=(float)(sum)/n;
   printf("Average score:%0.2f \n",avg);
   
   printf("Number of failure: %d\n",count);
 
   return 0;
}
