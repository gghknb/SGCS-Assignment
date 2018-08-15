#include <stdio.h>

void pull_char(char *arr);

int main()
{
    char arr[10]={'I','L','I','K','E','R','I','V','E','N'};
    char a;
    int i;
    
    printf("before  : ");
   
    for(i=0;i<10;i++)
    printf("%c ",*(arr+i));
    printf("\n\n");
  
    pull_char(arr);

    printf("after   : ");
    
    for(i=0;i<10;i++)
    printf("%c ",*(arr+i));
    
    printf("\n");  
    return 0;
}

void pull_char(char *arr)
{
    char a;
    int x,i,j=0;
    printf("target char : ");
    scanf("%c",&a);

    for(i=0;i<10;i++)
    {
      if(*(arr+i)!=a)
      {
        *(arr+j)=*(arr+i);
        j++;
      }
    }
    for(i=j;i<10;i++)
      *(arr+i)=a;
}

    
    
