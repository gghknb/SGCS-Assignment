#include <stdio.h>

void convert_digit(FILE *fp,int num);

int main()
{
        FILE *fp;
        int num;
        fp=fopen("output","w");
        printf("Input number: ");
        scanf("%d",&num);
        convert_digit(fp,num);
        fprintf(fp,"\n");
return 0;
}

void convert_digit(FILE *fp,int num)
{
        

        fprintf(fp,"Current number: %d\n",num);
        fprintf(fp,"Quotient : %d\n",num/8);
        fprintf(fp,"Remainder : %d\n",num%8);        
        if(num/8==0)
        {
           fprintf(fp,"Result : ");
           fprintf(fp,"%d",num%8);
           return;
        }
        convert_digit(fp,num/8);
        fprintf(fp,"%d",num%8);
        
}
