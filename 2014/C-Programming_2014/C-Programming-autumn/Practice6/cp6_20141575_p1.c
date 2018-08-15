#include <stdio.h>
#include <stdlib.h>

char *triple_concatenate(char *str1,char *str2,char *str3);

int main()
{
  char *str1,*str2,*str3,*result;
  int a,b,c,k;
  printf("Input size of str1 : ");
  scanf("%d",&a);
  getchar();
  printf("Input str1 : ");
  str1=(char*)malloc(a*sizeof(char));
  gets(str1);
  
  printf("Input size of str2 : ");
  scanf("%d",&b);
  getchar(); 
  printf("Input str2 : ");
  str2=(char *)malloc(b*sizeof(char));
  gets(str2);

  printf("Input size of str3 : ");
  scanf("%d",&c);
  getchar();
  printf("Input str3 : ");
  str3=(char *)malloc(c*sizeof(char));
  gets(str3);
  
  
  printf("str1   : %s\n",str1);

  printf("str2   : %s\n",str2);
  
  printf("str3   : %s\n",str3);
  result=triple_concatenate(str1,str2,str3);
  printf("result : %s\n",result);
  return 0;
}

char *triple_concatenate(char *str1,char *str2,char *str3)
{
   char *result;
   int x=0,y=0,z=0,a,i;
   for(i=0;;i++)
   {
     if(str1[i]=='\0')
       break;
     x++;
   }
   for(i=0;;i++)
   {
     if(str2[i]=='\0')
       break;
     y++;
   }
   for(i=0;;i++)
   {
     if(str3[i]=='\0')
       break;
     z++;
   }
   a=x+y+z+1;
   result=(char *)malloc(a*sizeof(char));
   for(i=0;i<x;i++)
   {
      *(result+i)=*(str1+i);
   }
   for(i=0;i<y;i++)
   {
     *(result+x+i)=*(str2+i);
   }
   for(i=0;i<z;i++)
   {
     *(result+x+y+i)=*(str3+i);
   }
   return result;
}



   
