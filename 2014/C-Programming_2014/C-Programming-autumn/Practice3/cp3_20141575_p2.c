#include <stdio.h>

void push_integer(int *arr);

int main()
{
    int arr[10]={99,87,75,63,51,43,37,29,13,1};
    int i; 
    printf("before : ");
    
    for(i=0;i<10;i++)
    printf("%d ",*(arr+i));
    printf("\n");

    push_integer(arr);
    
    printf("after : ");

    for(i=0;i<10;i++)
    printf("%d ",*(arr+i));
    printf("\n");

    return 0;
}

void push_integer(int *arr)
{
    int i,a,temp,x,t;
    printf("push integer : ");
    scanf("%d",&a);

    for(i=0;i<10;i++)
    {
       if(a>*(arr+i))
       {
         temp=*(arr+i);
         x=i;
         *(arr+i)=a;
         for(t=9;t>x+1;t--)
         {
            *(arr+t)=*(arr+t-1);
         }
         *(arr+x+1)=temp;
        
         break;
       }
    }       
}
    






