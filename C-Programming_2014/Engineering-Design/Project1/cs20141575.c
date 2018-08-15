#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

void taylor_sin(double, int, double *);
void taylor_cos(double, int, double *);
void taylor_tan(double, int, double *);
int factorial(int);
double angle_to_radian(double);
void compute_error_rate(double, double, double, double);

int main()  
{
       double angle,radian,tsin,tcos,ttan; //angle은 60분법 형태의 각도, radian은 라디안, tsin tcos ttan 은 각각 taylor series 를 이용해 구현한 sin cos tan 이다.     
       int terms; // terms 은 taylor series를 이용해 전개할때 항의 개수
            
       printf("Input the angle : "); 
       scanf("%lf",&angle);  //각을 입력 
       
       printf("Input the number of terms (3~5) : "); 
       scanf("%d",&terms);  //항을 입력
       if(3<=terms&&terms<=5)      //항의 개수가 3~5 사이가 아니면 오류가 뜬다는 조건문
       { 
       printf("\n<Sin, Cos, Tan function using math library>\n");
       radian=angle_to_radian(angle);     //angle 을 radian으로 바꿔준값을 radian 에 저장
       if(radian==-90*M_PI/180||radian==-270*M_PI/180||radian==90*M_PI/180||radian==270*M_PI/180) //angle 이 -90 -270,90,270도 일때  Tan 값이 Infinite 로 나타내도록 한 조건문
        {
        printf("Sin(%0.2lf) :%25.15lf\n",angle,sin(radian));
        printf("Cos(%0.2lf) :%25.15lf\n",angle,cos(radian));
        printf("Tan(%0.2lf) :          Infinite\n",angle);
        }
       else
        {
        printf("Sin(%0.2lf) :%25.15lf\n",angle,sin(radian));
        printf("Cos(%0.2lf) :%25.15lf\n",angle,cos(radian));
        printf("Tan(%0.2lf) :%25.15lf\n",angle,tan(radian));
        }
      
      
       printf("\n<Sin, Cos, Tan function using taylor series>\n");
       taylor_sin(radian,terms,&tsin);  //taylor series를 이용해 전개한 sin함수를 호출
       printf("Sin(%0.2lf) :%25.15lf\n",angle,tsin);      
       taylor_cos(radian,terms,&tcos);  ///taylor series를 이용해 전개한 cos함수를 호출
       printf("Cos(%0.2lf) :%25.15lf\n",angle,tcos);
       taylor_tan(radian,terms,&ttan);  ///taylor series를 이용해 전개한 tan함수를 호출
       printf("Tan(%0.2lf) :%25.15lf\n",angle,ttan);
      
       compute_error_rate(tsin,tcos,ttan,radian); //오차율 구하는 함수를 호출
      
      }
       else printf("error !\n");
     
       return 0;
}

void taylor_sin(double radian, int terms , double *tsin) //taylor series 를 이용해 sin값을 계산한 함수
{
     
    if(terms==3) //taylor series 로 3항까지 전개한 sin값
    *tsin=pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5);
    
    else if(terms==4) //taylor series 로 4항까지 전개한 sin값
    *tsin=pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5)+pow(-1,3)/factorial(7)*pow(radian,7);     
    
    else if(terms==5) //taylor seires 로 5항까지 전개한 sin 값
    *tsin=pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5)+pow(-1,3)/factorial(7)*pow(radian,7)+pow(-1,4)/factorial(9)*pow(radian,9);
    
}

void taylor_cos(double radian, int terms, double *tcos)  //taylor series 를 이용해 cos 값을 계산한 함수
{
    if(terms==3) //taylor series 로 3항까지 전개한 cos값
    *tcos=pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4); 
    
    else if(terms==4) //taylor series 로 4항까지 전개한 cos값
    *tcos=pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4)+pow(-1,3)/factorial(6)*pow(radian,6);
    
    else if(terms==5) //taylor series로 5항까지 전개한 cos값
    *tcos=pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4)+pow(-1,3)/factorial(6)*pow(radian,6)+pow(-1,4)/factorial(8)*pow(radian,8);
}

void taylor_tan(double radian, int terms, double *ttan) //taylor series 를 이용해 tan 값을 계산한 함수
{
     if(terms==3)  //taylor series로 3항까지 전개한 tan값
     *ttan=(pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5))/(pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4));
     
     else if(terms==4)  //taylor series로 4항까지 전개한 tan값
     *ttan=(pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5)+pow(-1,3)/factorial(7)*pow(radian,7))/(pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4)+pow(-1,3)/factorial(6)*pow(radian,6));
     
     else if(terms==5) //taylor series로 5항까지 전개한 tan값
     *ttan=(pow(-1,0)/factorial(1)*pow(radian,1)+pow(-1,1)/factorial(3)*pow(radian,3)+pow(-1,2)/factorial(5)*pow(radian,5)+pow(-1,3)/factorial(7)*pow(radian,7)+pow(-1,4)/factorial(9)*pow(radian,9))/(pow(-1,0)/factorial(0)*pow(radian,0)+pow(-1,1)/factorial(2)*pow(radian,2)+pow(-1,2)/factorial(4)*pow(radian,4)+pow(-1,3)/factorial(6)*pow(radian,6)+pow(-1,4)/factorial(8)*pow(radian,8));
     
}

int factorial(int fac) // factorial 값을 계산한 함수, taylor series를 5항까지만 전개하니 9! 까지만 계산 
{
      if(fac==2) return 1*2;
      else if(fac==3) return 1*2*3;
      else if(fac==4) return 1*2*3*4;
      else if(fac==5) return 1*2*3*4*5;
      else if(fac==6) return 1*2*3*4*5*6;
      else if(fac==7) return 1*2*3*4*5*6*7;
      else if(fac==8) return 1*2*3*4*5*6*7*8;
      else if(fac==9) return 1*2*3*4*5*6*7*8*9;
      else return 1;  
}
double angle_to_radian(double angle)  // angle 을 radian 으로 바꿔주는 함수
{
   double radian;
   if(angle>=360)   //angle 이 360도 보다 클때 -2PI~2PI 사이의 각으로 변환시켜주는 조건문
   {
   radian=(((int)(angle*100)%36000)/100)*M_PI/180; //double 형에서 %를 쓸수없으니 radian값을 int 형으로 바꾸어서 %를 써서 radian값을 구하였다
   return radian;
   }
   else if(angle<360&&angle>-360) //angle이 -2PI~2PI 에있으니 radian 값으로 변환해주는 조건문
   {
   radian=angle*M_PI/180;
   return radian;
   }
   else if(angle<=-360)  //angle 이 -360도 보다 작을때 -2PI~2PI 사이의 각으로 변환 시켜주는 조건문
   {
    radian=(((int)(angle*100)%36000)/100)*M_PI/180;
    return radian; 
   }
}

void compute_error_rate(double tsin, double tcos,double ttan, double radian) //오차율을 구하고 그것을 출력해주는 함수
{
   tsin=fabs((sin(radian)-tsin)/sin(radian)*100);  //math library로 얻은 sin과 taylor series로 얻은 sin의 오차율을 tsin에 저장
   tcos=fabs((cos(radian)-tcos)/cos(radian)*100);  //math library로 얻은 cos과 taylor series로 얻은 cos의 오차율을 tcos에 저장
   ttan=fabs((tan(radian)-ttan)/tan(radian)*100);  //math library로 얻은 tan와 taylor series로 얻은 tan의 오차율을 ttan에 저장
   if(radian==-180*M_PI/180||radian==0||radian==180*M_PI/180) //sin값이 0이될때 sin 의 오차율이 Infinite 가 나오게해주는 조건문
   printf("\nThe error rate of Sin :          Infinite\n");
   else
   printf("\nThe error rate of Sin :%25.13lf\n",tsin);
  
   if(radian==90*M_PI/180||radian==270*M_PI/180||radian==-90*M_PI/180||radian==-270*M_PI/180) //cos값이 0이될때 cos의 오차율이 Infinite 가 나오게해주는 조건문
   printf("The error rate of Cos :          Infinite\n");
   else
   printf("The error rate of Cos :%25.13lf\n",tcos);
  
   if(radian==0||radian==180*M_PI/180||radian==-180*M_PI/180||radian==90*M_PI/180||radian==270*M_PI/180||radian==-90*M_PI/180||radian==-270*M_PI/180)// tan값이 무한이되거나 cos값이 0이될때 tan의 오차율이 Infinite 가 나오게 해주는 조건문
   printf("The error rate of Tan :          Infinite\n");
   else
   printf("The error rate of Tan :%25.13lf\n",ttan);
}
