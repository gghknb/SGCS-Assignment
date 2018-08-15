#include <stdio.h>

void move_order(int *arr);
int main()
{
  int arr[10];
  int i;

  printf("input 10 integer : ");
  for(i=0;i<10;i++)
    scanf("%d",arr+i);

  printf("before : ");
  for(i=0;i<10;i++)
    printf("%d ",*(arr+i));
  printf("\n");

  move_order(arr);
  
  printf("after : ");
  for(i=0;i<10;i++)
    printf("%d ",*(arr+i));
  
  printf("\n");
  
  return 0;
}

void move_order(int *arr)
{
  int i;
  int temp;
  temp=*(arr+9);
  for(i=9;i>0;i--)
    *(arr+i)=*(arr+i-1);
  *arr=temp;
}

