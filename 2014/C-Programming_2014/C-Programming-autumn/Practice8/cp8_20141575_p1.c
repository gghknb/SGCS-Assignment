#include <stdio.h>
#include <stdlib.h>

int **multiple(int **matrixA,int **matrixB,int size);

int main()
{
  int **matrixA;
  int **matrixB;
  int **result;
  int n;
  int i,j;
  printf("Input matrix size N : ");
  scanf("%d",&n);
  srand((unsigned int)time(NULL));
  matrixA=(int **)malloc(sizeof(int *)*n);
  for(i=0;i<n;i++)
  {
    *(matrixA+i)=(int *)malloc(sizeof(int )*n);
  }

  matrixB=(int **)malloc(sizeof(int *)*n);
  for(i=0;i<n;i++)
  {
    *(matrixB+i)=(int *)malloc(sizeof(int)*n);
  }

  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      *(*(matrixA+i)+j)=rand()%99+1;
    }
  }
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      *(*(matrixB+i)+j)=rand()%100;
    }
  }
  result=multiple(matrixA,matrixB,n);
 
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      printf("%4d",*(*(matrixA+i)+j));
    }
    printf("\t\t");
    for(j=0;j<n;j++)
    {
      printf("%4d",*(*(matrixB+i)+j));
    }
    printf("\t\t");
    for(j=0;j<n;j++)
    {
      printf("%6d",(*(*result+i)+j));
    }
    printf("\n");
  }


  for(i=0;i<n;i++)
  {
    free(*(matrixA+i));
  }
  for(i=0;i<n;i++)
  {
    free(*(matrixB+i));
  }

  for(i=0;i<n;i++)
  {
    free(*(result+i));
  }

  free(matrixA);
  free(matrixB);
  free(result);
  return 0;
}

int **multiple(int **matrixA,int **matrixB,int size)
{
  int **result;
  int i,j,k;
  result=(int **)malloc(sizeof(int *)*size);
  for(i=0;i<size;i++)
  {
    *(result+i)=(int *)malloc(sizeof(int)*size);
  }
  for(i=0;i<size;i++)
  {
    for(j=0;j<size;j++)
    {
      for(k=0;k<size;k++)
      {
        *(*(result+i)+j)+=*(*(matrixA+i)+k) * *(*(matrixB+k)+j);
      }
    }
  }
  return result;
}

