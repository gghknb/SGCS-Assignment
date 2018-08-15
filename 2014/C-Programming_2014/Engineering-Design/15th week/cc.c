#include<stdio.h>


int main()
{
  FILE *fp1, *fp2, *fp3;
  char filename[100]={0};
  char ch1,ch2;
  char buffer1[10000]={0}, buffer2[10000]={0};
  int index1=0; int index2 = 0;
  int i;
  int match=0;
  printf("Input first file name : ");
  scanf("%s",filename);
  fp1 = fopen(filename,"r");
  
  printf("Input second file name : ");
  scanf("%s",filename);
  fp2 = fopen(filename,"r");
  fp3 = fopen("output","w");

  while(1)
  {
    ch1 = fgetc(fp1);
    ch2 = fgetc(fp2);
    if(ch1==EOF || ch2==EOF)
      break;
  
 
  if((ch1 != ' ' && ch1 !='\n') && (ch2 != ' ' && ch2 != '\n'))
  {
    buffer1[index1] = ch1; buffer2[index2] =ch2;
    index1++; index2++;
  }
  
  else if((ch1=='\n' || ch1==' ') && (ch2 !=' ' && ch2 != '\n'))
  {
    buffer2[index2]=ch2;
    index2++;
  }
  else if((ch1 != '\n' || ch1==' ') && (ch2==' ' && ch2=='\n')) 
  {
    buffer1[index1]=ch1;
    index1++;
  }

  else 
    continue;
 }
  if(index1 <index2)
    index1 =index2;


  for(i=0;i<index1;i++)
  {
    if(buffer1[i]==buffer2[i])
       match++; 
  }
  fprintf(fp3,"Total character: %d\n",index1);
  fprintf(fp3,"Same  character: %d\n",match);
  fprintf(fp3,"%.3f%%\n",(float)match/(float)index1*100);
  
  return 0;
}

