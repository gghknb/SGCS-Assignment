#include <stdio.h> 
#include <stdlib.h>

char* Encryption(char* Plaintext, int key);

int main()
{
  int key;
  char *text;
  char *encrypt;
  char *decrypt;
  int i,len=0;
  text=(char *)malloc(30*sizeof(char));
  printf("Input the plaintext : ");
  for(i=0;i<31;i++)
  {
    scanf("%c",text+i); 
    len++;
    if(*(text+i)== '\n')
      break;
  }
  printf("Input key : ");
  scanf("%d",&key);
  
  if(key<0||key>=26)
    return 0;
  
  printf("Encrypt(");
  for(i=0;i<len-1;i++)
  {
    printf("%c",*(text+i));
  }
  printf(") = ");
  encrypt=Encryption(text,key);
  for(i=0;i<len;i++)
  {
    printf("%c",*(encrypt+i));
  }
   
  printf("Decrypt(");
  for(i=0;i<len-1;i++)
  {
    printf("%c",*(encrypt+i));
  }
  printf(") = ");
  decrypt=Encryption(encrypt,26-key);
  for(i=0;i<len;i++)
  {
    printf("%c",*(decrypt+i));
  }

  return 0;
}

char* Encryption(char* Plaintext, int key)
{
  int i;
  char *encrypt=(char *)malloc(30*sizeof(char));
  for(i=0;i<31;i++)
  {

    if(*(Plaintext+i)>='a'&&*(Plaintext+i)<='z')
    {
      *(encrypt+i)=*(Plaintext+i)+key;
      if(*(encrypt+i)>'z'||*(encrypt+i)<='a')
      {
        *(encrypt+i)=*(encrypt+i)-26;
      }
    }
    else if(*(Plaintext+i)>='A'&&*(Plaintext+i)<='Z')
    {
      *(encrypt+i)=*(Plaintext+i)+key;
      if(*(encrypt+i)>'Z'||*(encrypt+i)<'A')
      {
        *(encrypt+i)=*(encrypt+i)-26;
      }
    }
    else
    {
     *(encrypt+i)=*(Plaintext+i); 
    }
  }
  return encrypt;
}
