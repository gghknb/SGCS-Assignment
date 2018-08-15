#include <stdio.h>
#include <stdlib.h>

int **transpose(int **matrix,int m,int n);

int main()
{
  int m,n;
  int **matrix;
  int i,j;
  int **result;
  printf("Number of Rows : ");
  scanf("%d",&m);
  printf("Number of Cols : ");
  scanf("%d",&n);
  srand((unsigned int)time(NULL));
  matrix=(int **)malloc(sizeof(int *)*n);
  for(i=0;i<n;i++)
  {
    *(matrix+i)=(int *)malloc(sizeof(int)*m);
  }
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
    {
      *(*(matrix+i)+j)=rand()%99+1;
    }
  }
  printf("Before : \n");
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
    {
      printf("%3d",*(*(matrix+i)+j));
    }
    printf("\n");
  }
  result=transpose(matrix,m,n);
  printf("After : \n");
  for(i=0;i<n;i++)
  {
    for(j=0;j<m;j++)
    {
      printf("%3d",*(*(result+i)+j));
    }
    printf("\n");
  }
  for(i=0;i<n;i++)
  {
    free(*(matrix+i));
  }
  free(matrix);
  for(i=0;i<m;i++)
  {
    free(*(result+i));
  }
  free(result);
  return 0;
}

int **transpose(int **matrix,int m,int n)
{
  int **result;
  int i,j;
  result=(int **)malloc(sizeof(int *)*n);
  for(i=0;i<n;i++)
  {
    *(result+i)=(int *)malloc(sizeof(int)*m);
  }
  for(i=0;i<n;i++)
  {
    for(j=0;j<m;j++)
    {

      *(*(result+i)+j)=*(*(matrix+j)+i);
    }
  }
  return result;
}



