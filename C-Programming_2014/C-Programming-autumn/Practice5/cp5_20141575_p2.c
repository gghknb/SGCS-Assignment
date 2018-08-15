#include <stdio.h>

void bubble_sort(int *data_array_row);

int main()
{
  int data_array[4][10]={ {838, 758, 113, 515, 51, 627, 10, 419, 535, 280},
    {212, 86, 749, 767, 84, 60, 225, 543, 427, 104},
    { 89, 183, 137, 566, 966, 978, 495, 311, 953, 827},
    {367, 54, 31, 145, 882, 736, 524, 505, 343, 639} };
  int i,j;
  int row_index;
  printf("Before sorting\n");
  for(i=0;i<4;i++)
  {
    for(j=0;j<10;j++)
    {
     printf("  %d ",*(*(data_array+i)+j));
    }
    printf("\n");
  }

  printf("Enter the row index (0~3) : ");
  scanf("%d",&row_index);
  bubble_sort(*(data_array+row_index));
  printf("After sorting\n");
  for(i=0;i<4;i++)
  {
    for(j=0;j<10;j++)
    {
      printf("  %d ",*(*(data_array+i)+j));
    }
    printf("\n");
  }
  return 0;
}


void bubble_sort(int *data_array_row)
{
  int i,j;
  int temp;
  for(j=0;j<9;j++)
  {
    for(i=0;i<9-j;i++)
    {
      if(*(data_array_row+i)>*(data_array_row+i+1))
      {
        temp=*(data_array_row+i);
        *(data_array_row+i)=*(data_array_row+i+1);
        *(data_array_row+i+1)=temp;
      }
    }
  }
}






