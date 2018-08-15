#include <stdio.h>

void swap(int *c,int *d);

int main()
{
  int x[10];
  int M,n,i,temp1,temp2;
   
  printf("Input (10numbers): ");
  
  for(i=0;i<10;i++)
  {
    scanf("%d",&x[i]);
   
 }

    temp2=x[0];
    temp2=x[0];
    for(i=0;i<10;i++)
    {
    if(temp1<x[i])
      temp1=x[i];
    if(temp2>=x[i])
       temp2=x[i];      
     
    }


  

  for(i=0; i<5; i++)
    swap(&x[i],&x[9-i]);

  for(i=0;i<10;i++)
  {
     printf("%d ",x[i]);
  }

  printf("\nMax: %d, Min:%d\n",temp1,temp2);


  return 0;
}


void swap(int *c,int *d)
{
  int a;
  a=*c;
  *c=*d;
  *d=a;
}





