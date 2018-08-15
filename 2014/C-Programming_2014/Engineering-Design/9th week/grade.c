#include <stdio.h>

void avgFunc(float a,float b,float c);
void gradeFunc(float average);

int main()
{
   float a,b,c;
   printf("Input: ");
   scanf("%f %f %f",&a,&b,&c);
   
   avgFunc(a,b,c); 

return 0;
}

void avgFunc(float a,float b,float c)
{
  float average=(a+b+c)/3;

  gradeFunc(average);
}

void gradeFunc(float average)
{
   
    if(average>=50)
    {
         if(average>=70)
            {
               if(average>=80)
                      {  if(average>=90)
                         printf("Grade: A\n");
                         else
                         printf("Grade: B\n");
                      }
                    
                 else
                 printf("Grade: C\n");
            }
         else
         printf("Grade: D\n");
    }
    else 
    printf("Grade: F\n");
} 





