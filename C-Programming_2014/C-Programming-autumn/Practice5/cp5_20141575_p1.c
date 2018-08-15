#include <stdio.h>

void sub_matrix(double *left_row, double *right_row, double *result_row, int n);
void rounding_off(double *row, int n);
int main()
{
  double left[5][5]={{1.4, 9.8, 2.5, 1.1, 5.7}, {1.2, 2.1, 8.7, 1.9, 7.5}, {3.2, 3.1, 3.7, 2.7, 3.9},
    {6, 8, 4.6, 0, 6.3}, {9.4, 6.6, 1.4, 6.7, 8.9}};
  double right[5][5]={{7, 4.8, 4.5, 7.1, 2.6}, {4.3, 2.6, 5.6, 8.2, 9.4}, {1.3, 4.1, 8.4, 7.9, 1.8},
    {5.2, 6.2, 8.5, 9, 1.8}, {3.4, 0.6, 8.9, 3.8, 2.1}};
  double result[5][5]={0};
  int i,j;

  printf("\tLeft\t\t\tRight\t\t\tResult\n");
  for(i=0;i<5;i++)
  {
   sub_matrix(*(left+i),*(right+i),*(result+i),5); 
  }
  
  for(i=0;i<5;i++)
  {
    for(j=0;j<5;j++)
    {
      printf("%3.1lf ",*(*(left+i)+j));
    }
    printf("\t");
    for(j=0;j<5;j++)
    {
      printf("%3.1lf ",*(*(right+i)+j));
    }
    printf("\t");
    for(j=0;j<5;j++)
    {
      printf("%3.1lf ",*(*(result+i)+j));
    }
    printf("\n");
  }

      
  return 0;
}

void sub_matrix(double *left_row, double *right_row, double *result_row, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
      *(result_row+i)=*(left_row+i)-*(right_row+i);
    }
      rounding_off(result_row,5);
   return;
}

void rounding_off(double *row, int n)
{
  int i;
  for(i=0;i<n;i++)
  {
    if(*(row+i)>0)
    {
      if(*(row+i)-(int)*(row+i)>=0.5)
         *(row+i)=(int)*(row+i)+1;
      else
         *(row+i)=(int)*(row+i);
    }
    else if(*(row+i)<0)
    {
      if(*(row+i)-(int)*(row+i)>-0.5)
        *(row+i)=(int)*(row+i);
      else
        *(row+i)=(int)*(row+i)-1;
    }
  }
  return;
}






















