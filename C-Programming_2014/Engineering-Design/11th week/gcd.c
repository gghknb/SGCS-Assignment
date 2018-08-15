#include <stdio.h>

void func_gcd(int,int,int*);

int main()
{
    int a,b,gcd;
    printf("Input first number: ");
    scanf("%d",&a);
    printf("Input seconde number: ");
    scanf("%d",&b);
    func_gcd(a,b,&gcd);
    printf("GCD: %d\n",gcd);
    return 0;
}

void func_gcd(int a,int b,int *gcd)
{
    int c,rem;
    if(a>b)
    {
      c=a;
      a=b;
      b=c;
    }
    while(1)
    {
      rem=a%b;
      a=b;
      b=rem;

      if(rem==0)
      {
        *gcd=a;
        break;
      }
    }
}


  
