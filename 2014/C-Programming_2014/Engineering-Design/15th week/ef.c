#include <stdio.h>
#include <math.h>

double change(char*, int ,int *);

int main()
{
        FILE *exfo;
        char arr[20];
        int x,l,i,t,digit;
        double result;

        exfo=fopen("input.txt","r");   
        x=fgetc(exfo)-'0';
        fgetc(exfo); 
      
        for(i=0;i<x;i++)
        { 
                for(t=0;t<20;t++)
                {
                        arr[t]=fgetc(exfo);
                        if(arr[t]=='\n')
                       {
                        l=t;
                        break;
                       }
               }


        result=change(arr,l,&digit);
         
        printf("%0.*lf\n",digit,result);
                 
       

        for(t=0;t<20;t++)
                arr[t]='\0';
       }
       fclose(exfo);

         
        return 0;
}

double change(char arr[],int l,int *digit)
{
        int k=0,a,n,v,q,t,mul=0,res,z=0;
        double b,m=0;
        for(t=0;t<l;t++)
         {
            if(arr[t]=='e'||arr[t]=='E')
                n=t;
         }
        for(t=0;t<l;t++) 
        {
                if(arr[t]=='.')
                        z=t;
        }


        
        if(arr[0]=='-')
        {
                for(t=1;t<n;t++)
                { 
                        if(arr[t]=='.')
                             break;        
                        k=k*10+(arr[t]-'0');
                        
                }
               if(arr[t]=='.')
               {
               for(a=t+1;a<n;a++)
                        m=m*10+(arr[a]-'0');
                
                
                b=k+(m/pow(10,a-t-1));
                }
                else b=(double)k;

                v=n+1;

                if(arr[v]=='+')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                             
                        b= b*pow(10,mul);
                    res=mul;
               }
                else if(arr[v]=='-')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                               
                        b=b/pow(10,mul);
                   res=(-1)*mul;
               }
                else
                {
                        for(q=v;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                        
                        b=b*pow(10,mul);
                  res=mul;
               }

               b=b*(-1);
          

      }
        else if(arr[0]=='+')
        {
                for(t=1;t<n;t++)
                {
                        if(arr[t]=='.')
                                break;
                        k=k*10+(arr[t]-'0');
                }
               if(arr[t]=='.')
               {

               for(a=t+1;a<n;a++)
                        m=m*10+(arr[a]-'0');

        
                b=k+(m/pow(10,a-t-1));
                }
                else
                        b=(double)k;
                
                v=n+1;

                if(arr[v]=='+')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');

                        b= b*pow(10,mul);
                        res=mul;
               }
                else if(arr[v]=='-')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                            
                        b=b/pow(10,mul);
                       res=(-1)*mul;
                }
                else 
                {
                        for(q=v;q<l;q++)
                                mul=mul*10+(arr[q]-'0');

                        b=b*pow(10,mul);
                    res=mul;
                }
            


        }
        else if(arr[0]!='+' && arr[0]!='-')
        {
                for(t=0;t<n;t++)
                {
                        if(arr[t]=='.')
                                break;
                        
                        k=k*10+(arr[t]-'0');
                }
               if(arr[t]=='.')
               {
                for(a=t+1;a<n;a++)
                        m=m*10+(arr[a]-'0');

                 b=k+(m/pow(10,a-t-1));
               }
                else
                        b=(double)k;
               
               v=n+1;

                if(arr[v]=='+')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                                              
                        b= b*pow(10,mul);
                  res=mul;
               }
                else if(arr[v]=='-')
                {
                        for(q=v+1;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                               

                        b=b/pow(10,mul);
                  res=(-1)*mul;
                }
                else
                {
                        for(q=v;q<l;q++)
                                mul=mul*10+(arr[q]-'0');
                               

                        b=b*pow(10,mul);
                 res=mul;
                }
              
        
            

         }
         
   *digit=n-z-res-1;
            if(*digit<0)
                    *digit=0;

     return b;     
}



