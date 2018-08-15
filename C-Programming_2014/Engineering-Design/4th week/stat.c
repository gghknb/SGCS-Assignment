#include <stdio.h>

float faverage(int a,int b,int c,int d);
float fdeviation(int x,float average);

int main()
{ int a,b,c,d;
  float average;
    printf("Enter the first number : ");
    scanf("%d",&a);
    printf("Enter the second  number : ");
    scanf("%d",&b);
    printf("Enter the third number : ");
    scanf("%d",&c);
    printf("Enter the fourth  number : ");
    scanf("%d",&d);
    average=faverage(a,b,c,d);
    printf("******** average is  %.2f ********\n",average);
    printf("first number :  %18d -- deviation : %10.2f\n",a,fdeviation(a,average));     
    printf("second number : %18d -- deviation : %10.2f\n",b,fdeviation(b,average));
    printf("third number :  %18d -- deviation : %10.2f\n",c,fdeviation(c,average));
    printf("fourth number : %18d -- deviation : %10.2f\n",d,fdeviation(d,average));
    return 0;
}

float faverage(int a,int b,int c,int d)
{
     float average = (float)(a+b+c+d)/4;
     return average;
}

float fdeviation(int x,float average)
{
     float deviation = x-average;
     return deviation;
}
