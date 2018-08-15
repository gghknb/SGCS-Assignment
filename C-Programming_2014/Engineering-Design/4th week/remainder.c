#include <stdio.h>

void divideRemainder(int* a,int * b);

int main()
{              
     int a,b;
     printf("Input two number : ");
     scanf("%d %d",&a,&b);
     printf("-------------------------------------\n");
     divideRemainder(&a,&b);
     printf("OUTPUT : divide [%d] remainder [%d] \n",a,b);
     return 0;
}

void divideRemainder(int* a,int* b)
{  
    int c= (*a) / (*b);
    *b= (*a) % (*b);
    *a=c;
    
}
