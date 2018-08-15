#include <stdio.h>

void bubble_sort(int *arr);

int main()
{
  int arr[10];
  int i;
  printf("type 10 integer : ");
  
  for(i=0;i<10;i++)
  scanf("%d",arr+i);
  
  for(i=0;i<10;i++)
  printf("%d ",*(arr+i));
  printf("\n");

  bubble_sort(arr);

  for(i=0;i<10;i++)
  printf("%d ",*(arr+i));
  printf("\n");

  return 0;

}

void bubble_sort(int *arr)
{
  int i,j;
  int temp;
  for(j=1;j<10;j++)
  {
    for(i=1;i<10-j;i++)
    {
      if(*(arr+10-i)<*(arr+j))
      {
        temp=*(arr+10-i);
        *(arr+10-i)=*(arr+j);
        *(arr+j)=temp;
      }
    }
  }
}
