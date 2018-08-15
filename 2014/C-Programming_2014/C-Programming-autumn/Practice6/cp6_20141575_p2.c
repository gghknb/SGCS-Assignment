#include <stdio.h>
#include <stdlib.h>
int *calculate_next(int *pascal_tr,int current_level);

int main()
{
  int a,i,j;
  int* pascal_tr;
  pascal_tr=(int*)malloc(sizeof(int));
  pascal_tr[0]=1;
  printf("Input the level of Pascal's triangle : ");
  scanf("%d",&a);
  for(i=1;i<=a;i++){
    pascal_tr=calculate_next(pascal_tr,i);
    for(j=0;j<i;j++){
      printf("%d ",pascal_tr[j]);
    }
    printf("\n");
  }
  return 0;
}

int *calculate_next(int *pascal_tr,int current_level) 
{
  int* next_level;
  int i;
  next_level=(int *)malloc((current_level)*sizeof(int));
  next_level[0]=1;
  next_level[current_level-1]=1;
 
  if(current_level!=1)
    for(i=1;i<current_level-1;i++){
       next_level[i]=pascal_tr[i]+pascal_tr[i-1];
     }
  return next_level;
}
