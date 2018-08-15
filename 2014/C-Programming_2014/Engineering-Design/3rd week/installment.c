#include <stdio.h>

int main (void)
{
         int a;
         int b;

    printf("price : ");
    scanf("%d",&a);
    printf("----------------output---------------\n");
    printf("Month\t\t:%10d%10d\n",3,6);
    b=a*1.05*1.05*1.05;
    printf("Payment\t\t:%10d%10d\n",a,b);
    printf("-------------------------------------\n");
    printf("Pay/month\t:%10.2f%10.2f\n",a/3.0,b/6.0);
    return 0;
}     
