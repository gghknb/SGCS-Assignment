#include <stdio.h>

int main()
{
   float a;
    
   printf("Input: ");
   scanf("%f",&a);
   printf("Use \">\": %d\n",(a-(int)a)>0.499999);
   printf("Use \">=\": %d\n",(a-(int)a)>=0.5);
   printf("Use \"==\": %d\n",(int)(a-(int)a+0.5)==1);
   printf("Use \"!=\": %d\n",(int)(a-(int)a+0.5)!=0);
   
   return 0;
}
