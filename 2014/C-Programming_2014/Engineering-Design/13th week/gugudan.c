#include <stdio.h>

int main()
{
        int x[10][10],i,k;
        for(i=0;i<10;i++)
        {
         printf("A[%d]",i); 
         for(k=0;k<10;k++)
          {
          x[i][k]=(i+1)*(k+1);
          printf("%5d",x[i][k]);      
          }
        printf("\n");
        }
        return 0;
}
