#include <stdio.h>
#include <stdlib.h>


char* getScientificFormatString(char* strPtr);

int main()
{
  char *strPtr;
  strPtr=(char *)malloc(sizeof(char)*16);
  printf("Input scientific format number : ");
  scanf("%15[0123456789.Ee+-]",strPtr);  
  
  if(getScientificFormatString(strPtr)==NULL)
  {
    printf("|%-20.15s|\n",strPtr);
  }
  else 
  {
    printf("|%20.15s|\n",strPtr);
  }
  
  free(strPtr);
  return 0;
}

char* getScientificFormatString(char* strPtr)
{
  int i;
  for(i=0;i<15;i++)
  {
    if(*(strPtr+i)=='E'||*(strPtr+i)=='e')
    {
      if(*(strPtr+i+1)=='+'||*(strPtr+i)=='-')
      {
        if(*(strPtr+i+2)>='0'&&*(strPtr+i+2)<='9')
        {
          if(*(strPtr+i+3)>='0'&&*(strPtr+i+3)<='9')
          {
            return strPtr;
          }
          else return NULL;
        }
        else return NULL;
      }
      else return NULL;
    }
  }
}
  
  
