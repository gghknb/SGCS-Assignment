#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiple(int left[4][4],int right[4][4],int result[4][4]);

int main()
{
  int left[4][4],right[4][4],result[4][4];
  int i,j;

  srand(time(NULL));
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      *(*(left+i)+j)=rand()%10;
      *(*(right+i)+j)=rand()%10;
      *(*(result+i)+j)=0;
    }
  }
  multiple(left,right,result);
  
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      printf("%d ",*(*(left+i)+j)); 
    }
    printf("\t");
    
    for(j=0;j<4;j++)
    {
      printf("%d ",*(*(right+i)+j));
    }
    printf("\t");
    
    for(j=0;j<4;j++)
    {
      printf("%d ",*(*(result+i)+j));
    }
    
    printf("\n");
  }
  return 0;
}

void multiple(int left[4][4],int right[4][4],int result[4][4])
{
  int i,j,k;
  
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      for(k=0;k<4;k++)
        *(*(result+i)+j)+=(*(*(left+i)+k))*(*(*(right+k)+j));
    }
  }
}

