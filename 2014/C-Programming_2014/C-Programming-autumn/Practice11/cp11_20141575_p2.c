#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void splitString(char *str,char *word1,char *word2);

int main()
{
	int i;
	char *str,*word1,*word2;
	str=(char *)malloc(sizeof(char)*30);
	word1=(char *)malloc(sizeof(char)*15);
	word2=(char *)malloc(sizeof(char)*15);
		
	printf("input str : ");
	gets(str);


	splitString(str,word1,word2);
	printf("[%s] -> [%s] [%s]\n",str,word1,word2);

	return 0;
}

void splitString(char *str,char *word1,char *word2){
	int i,n,len1,len2;
	n=strlen(str);
	len1=n/2;
	if(n%2==1)
		len2=len1+1;
	else
		len2=len1;

	strncpy(word1,str,len1);
	strncpy(word2,str+len1,len2);
	strcat(word1,"\0");
	strcat(word2,"\0");
}
