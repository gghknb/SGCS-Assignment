#include <stdio.h>
#include <math.h>

double charchange(char x[],int i);

int main()
{ 
        char x[10];
        int i;
        for(i=0;i<10;i++)
        { 
         scanf("%c",&x[i]);
         if(x[i]=='\n')
                 break;
        }
        
         printf("Result: %lf\n",charchange(x,i));
  
   return 0;
}

double charchange(char x[],int i)
{
    int t=0,q;
    int k=0;
    double m=0,a,b;
    if(x[t]=='0')
      b=0;       
   
   else if(x[t]=='-')
     {
        for(t=1;t<10;t++)
     {
     if(x[t]=='.')
        break;
        k=k*10+(x[t]-48);
     }
      for(q=t+1;q<i;q++)
        m=m*10+(x[q]-48);
             
        a=m/pow(10,q-t-1);
         b=k+a;
         b=(-1)*b;

    }    
    else
   {
    
    for(t=0;t<10;t++) 
    {
      if(x[t]=='.')
      break;
     k=k*10+(x[t]-48);
    }
    for(q=t+1;q<i;q++)       
     
     m=m*10+(x[q]-48);
    
     a=m/pow(10,q-t-1); 
     b=k+a;
   }
   
    
        return b;
}
