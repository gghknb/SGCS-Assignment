# include<stdio.h>
void swap(float*n1,float *n2);

int main()
{
  float a,b;
  printf("Input two number : \n");
  printf("        x : ");
  scanf("%f",&a);
  printf("        y : ");
  scanf("%f",&b);
  printf("-----------------------------------------\n");
  printf("Before : x [%.0f] y [%.0f]\n",a,b);
  printf("-----------------------------------------\n");
  swap(&a,&b);
  printf("After  : x [%.0f] y [%.0f]\n",a,b);
  return 0;
}

void swap(float *n1,float *n2) 
{
  float z=*n1;
  *n1=*n2;
  *n2=z;
}
