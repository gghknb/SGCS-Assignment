#include <stdio.h>

void atoupper(char x[]);

int main()
{ 
    char x[50];
    int i,k;
    for(i=0;i<50;i++)
    {
    
     scanf("%c",&x[i]);
    
    if(x[i]=='\n')
       
       break;
    }
    k=i;
   
    if(k>50)
     k=50;

      atoupper(x);

   for(i=0;i<k;i++)  
      printf("%c",x[i]);
       
     
    printf("\n");
    
return 0;
}

void atoupper(char x[])
{ 
   int n;
     
    for(n=0;n<50;n++)
    if(x[n]>=97&&x[n]<=122)
    x[n]=x[n]-32;
  
}
