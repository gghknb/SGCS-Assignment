#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *new_strncpy(char **to_string,char *from_string,int size);

int main()
{
	int size,i,len;
	char *to_string,*from_string;
	from_string=(char *)malloc(sizeof(char)*1024);
	printf("input string : ");
	gets(from_string);
	printf("input size : ");
	scanf("%d",&size);
	len=strlen(from_string);
	to_string=(char *)malloc(sizeof(char)*len);
	to_string=NULL;
	printf("before p = %s\n",to_string);
	new_strncpy(&to_string,from_string,size);
	printf("After p = %s\n",to_string);
	return 0;
}

char *new_strncpy(char **to_string,char *from_string,int size)
{
	int len,i;
	len=strlen(from_string);
	*to_string=(char *)malloc(sizeof(char)*len);
	if(*(from_string+0)=='\0')
	{
		(*to_string)=NULL;
		return 0;
	}
	if(size==0)
	{
		strcpy(*to_string,from_string);
		return 0;
		
	}
	if(size<=len)
	{
		strncpy(*to_string,from_string,size);
	}
	else
	{
		strncpy(*(to_string),from_string,len);
		*(*to_string+size)='\0';
	}
	
	return NULL;
}


	

