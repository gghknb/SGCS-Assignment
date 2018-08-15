#include <stdio.h>

float xcal(int a,int b,int c,int d);
float ycal(int a,int b,int c,int d);

int main()
{   
      int a, b, c, d;
      float x,y;
      printf("1st equation: y = ax - b, input 'a' and 'b'\n");
      printf("a = ");
      scanf("%d",&a);
      printf("b = ");
      scanf("%d",&b);
      printf("2nd equation: y = cx - d, input 'c' and 'd'\n");
      printf("c = ");
      scanf("%d",&c); 
      printf("d = ");
      scanf("%d",&d); 
      printf("result:\n");
      x=xcal(a,b,c,d); 
      printf("x is %f\n",x);
      y=ycal(a,b,c,d);
      printf("y is %f\n",y);
    return 0;
}

float xcal(int a, int b, int c, int d)
{
     float x=(float)(b-d)/(float)(a-c);
     return x; 
}

float ycal(int a, int b, int c, int d)
{  
     float y=(float)(a*(b-d)/(float)(a-c)-b);
     return y;
}
