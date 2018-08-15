#include <stdio.h>

void matrixmul(int x[3][4][4],int i,int j,int k);

int main()
{
        int x[3][4][4],i,j,k;
        printf("first matrix: \n");
       
        for(i=0;i<4;i++)
         {
            for(j=0;j<4;j++)
                x[2][i][j]=0;
         }       
       for(i=0;i<4;i++)   
         {
          for(j=0;j<4;j++)
           scanf("%d",&x[0][i][j]);

         }
       
       printf("second matrix: \n");       
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
            scanf("%d",&x[1][i][j]);
         
        
        }
        matrixmul(x,i,j,k);
        printf("Result: \n");
      for(i=0;i<4;i++)
          {
          for(j=0;j<4;j++)
           { 
            printf("%3d",x[2][i][j]);
            printf("\t");
           }
           printf("\n");
          }
  return 0;
}

void matrixmul(int x[3][4][4],int i,int j,int k)
{
      
      for(i=0;i<4;i++)
       {
        for(j=0;j<4;j++)
         {
          for(k=0;k<4;k++) 
            x[2][i][j]+=x[0][i][k]*x[1][k][j]; 
         }
       }
    

}
