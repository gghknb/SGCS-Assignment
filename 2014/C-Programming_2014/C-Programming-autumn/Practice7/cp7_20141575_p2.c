#include <stdio.h>
#include <stdlib.h>

int *merge(int *ary1,int size1,int *ary2,int size2,int *ary3,int size3);

int main()
{
  int *ary1=(int *)calloc(3,sizeof(int));
  int *ary2=(int *)calloc(4,sizeof(int));
  int *ary3=(int *)calloc(5,sizeof(int));
  int i,a;
  int size1=3,size2=4,size3=5;
  int *result;
  printf("input 12 integer : ");
  for(i=0;i<3;i++)
  {
    scanf("%d",ary1+i);
  }
  for(i=0;i<4;i++)
  {
    scanf("%d",ary2+i);
  }
  for(i=0;i<5;i++)
  {
    scanf("%d",ary3+i);
  }
  printf("Array1            : ");
  for(i=0;i<size1;i++)
    printf("%d ",*(ary1+i));
  printf("\n");
  printf("Array2            : ");
  for(i=0;i<size2;i++)
    printf("%d ",*(ary2+i));
  printf("\n");
  printf("Array3            : ");
  for(i=0;i<size3;i++)
    printf("%d ",*(ary3+i));
  printf("\n");
  result=merge(ary1,3,ary2,4,ary3,5);
  printf("result Array : ");
  for(i=0;i<size1+size2+size3;i++)
    printf("%d ",*(result+i));
  printf("\n");
  free(ary1);
  free(ary2);
  free(ary3);
  free(result);
  return 0;  
}

int *merge(int *ary1,int size1,int *ary2,int size2,int *ary3,int size3)
{
  int *result,temp;
  int i,j;
  result=(int *)calloc(size1+size2+size3,sizeof(int));
  for(i=0;i<size1;i++)
    *(result+i)=*(ary1+i);
  for(i=size1;i<size1+size2;i++)
    *(result+i)=*(ary2+i-size1);
  for(i=size1+size2;i<size1+size2+size3;i++)
    *(result+i)=*(ary3+i-size1-size2);
  for(i=0;i<size1+size2+size3;i++)
  {
    for(j=i+1;j<size1+size2+size3;j++)
    {
      if(*(result+i)>*(result+j))
      {
        temp = *(result+i);
        *(result+i) = *(result+j);
        *(result+j) = temp;
      }
    }
  }
  return result;
}

    

