#include <stdio.h>

int main()
{
   float a,b;
   int x;
   printf("Please Input two number: ");
   scanf("%f %f",&a,&b);
   printf("**********************\n");
   printf("* 1. Addition        *\n");
   printf("* 2. subtraction     *\n");
   printf("* 3. Multiplication  *\n");
   printf("* 4. Division        *\n");
   printf("**********************\n");
   printf("Select menu ");
   scanf("%d",&x);
   switch(x) 
   {
     case 1 : printf("a + b = %f\n",a+b); break;
     case 2 : printf("a - b = %f\n",a-b); break;
     case 3 : printf("a * b = %f\n",a*b); break;
     case 4 : switch((int)(100000*b))
     { 
        case 0 : printf("error! b cannot be 0\n");break;
        default : printf("a / b = %f\n",a/b);
     } break;
     default : printf("error ! \n");
   }   
  return 0;
}
