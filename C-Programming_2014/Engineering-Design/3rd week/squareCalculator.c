#include  <stdio.h>

int sqr();
int printOne();

int main(void)
{  
  int a;     
  printf("Input: ");
  scanf("%d",&a);
  sqr(a);
  return 0;     
}

int sqr(int a)
{ 
  int result;
  result=a*a;
  printOne(result);
  return result; 
}

int printOne(int result)
{ 
   printf("Result: %d \n",result);
   return 0;
}
