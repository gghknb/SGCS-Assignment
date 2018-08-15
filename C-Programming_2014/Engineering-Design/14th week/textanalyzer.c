#include <stdio.h>

void textanalyzer(int arr[],FILE *file);

int main(void)
{
         int arr[26]={0};
         int x;      
         FILE* file;
         file = fopen("text.txt","r");
         textanalyzer(arr,file);
         for(x=0;x<26;x++)
         {
                 printf("%c(%c):%4d,   ",x+'a',x+'A',arr[x]);
                 if(x%6==5||x==25)
                         printf("\n");
         }
        
         fclose(file);
        return 0;
}

void textanalyzer(int arr[],FILE *file)
{
        int y;
        while(y!=EOF)
        {
           y=fgetc(file);
           if(y>='a'&&y<='z')
           arr[y-'a']++;
           else if(y>='A'&&y<='Z') 
           arr[y-'A']++;
        }
   

}


