#include "20141575.h"

int main()
{
  int n,i,j; 
  double **matrix; 
  double **inmatrix; //Theorem1,2 로 구한 역행렬
  double **E; // inmatrix 와 matrix 의 곱
  double **L; //Lower triangular matrix
  double **U; //Upper triangular matrix
  double **LU; //Lower triangular matrix 와 Upper triangular matrix 의 곱
  double **E1; //Gaussian Elimination 으로 구한 역행렬과 matrix 의 곱
  double **inmatrix_gaussian; // Gaussian Elimination 으로 구한 역행렬
  while(1)
  {
    printf("system> input size n : "); //n을 입력받는다.
    scanf("%d",&n); 
    if(n<=0)
      return 0;
    else if(n>5) continue;
    printf("system> %d * %d elements\n",n,n);
    matrix=(double  **)malloc(sizeof(double *)*n); //출력하는데 필요한 행렬들을 할당해준다.
    inmatrix=(double **)malloc(sizeof(double *)*n);
    E=(double **)malloc(sizeof(double *)*n);
    E1=(double **)malloc(sizeof(double *)*n);
    L=(double **)malloc(sizeof(double *)*n);
    U=(double **)malloc(sizeof(double *)*n);
    LU=(double **)malloc(sizeof(double *)*n);
    inmatrix_gaussian=(double **)malloc(sizeof(double *)*n);
    for(i=0;i<n;i++)
    {
      *(matrix+i)=(double *)malloc(sizeof(double )*n);
      *(inmatrix+i)=(double *)malloc(sizeof(double)*n);
      *(E+i)=(double *)malloc(sizeof(double)*n);
      *(L+i)=(double *)malloc(sizeof(double)*n);
      *(U+i)=(double *)malloc(sizeof(double)*n);
      *(LU+i)=(double *)malloc(sizeof(double)*n);
      *(inmatrix_gaussian+i)=(double *)malloc(sizeof(double)*n);
      *(E1+i)=(double *)malloc(sizeof(double)*n);
    }
    for(i=0;i<n;i++) // matrix 를 입력받는다.
    {
      for(j=0;j<n;j++)
      {
        scanf("%lf",(*(matrix+i)+j));
      }
    }
    printf("system> Input matrix A\n"); //입력받은 matrix를 출력한다.
    for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
      {
        printf("%7.2lf ",*(*(matrix+i)+j));
      }
      printf("\n");
    }
    if(n<4) //n이 1이거나 2이거나 3일경우
    {
      inmatrix=inverseA(matrix,n); //Theorem1,2 로 구한 역행렬을 리턴해주는 함수호출
      if(detA(matrix,n)==0)  //detA가 0일때는 역행렬이 없고 detA가 0이아닐때에는 역행렬이 존재한다.
      { 
        printf("system> A is singular\n");
        continue;
      }
      else
      {
        printf("system> A is nonsingular\n");
        printf("system> inverse of matrixA(Theorem 2)\n"); // Theorem1,2 로 구한 역행렬을 출력
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf ",*(*(inmatrix+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n");
        E=mulMatrix(matrix,inmatrix,n);  // Theorem1,2로 구한 역행렬과 matrix 의 곱을 출력
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf ",*(*(E+i)+j));
          }
          printf("\n");
        }
        printf("system> inverse of matrix A(Gaussian Elimination)\n"); //Gaussian Elimination으로 구한 역행렬을 출력
        inmatrix_gaussian=inverseA_gaussian(matrix,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(inmatrix_gaussian+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n"); // Gaussian Elimination으로 구한 역행렬과 matrix 의 곱을 출력
        E1=mulMatrix(matrix,inmatrix_gaussian,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(E1+i)+j));
          }
          printf("\n");
        }
        LU=matrix_LU(matrix,L,U,n); // L과 U의 곱을 리턴해주는 함수호출
        if(LU==NULL)
		{
			printf("system> Not Available (Matrix Triangularization)\n");
			continue;
		}
		printf("system> Lower triangular matrix\n"); //Lower triangular matrix 를 출력
		for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(L+i)+j));
          }
          printf("\n");
        }
        printf("system> Upper triangular matrix\n"); //Upper triangular matrix 를 출력
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(U+i)+j));
          }
          printf("\n");
        }
        printf("system> L*U\n"); // Lower triangular matrix 와 Upper triangular matrix 의 곱을 출력
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(LU+i)+j));
          }
          printf("\n");
        }
      }
    }
    else if(n==4) // n이 4일 경우
    {
      inmatrix=inverseA(matrix,4);
      if(detA44(matrix)==0||detA4(matrix)==0)
      {
        printf("system> A is singular\n");
        continue;
      }
      else
      {
        printf("system> A is nonsingular\n");
        printf("system> inverse of matrixA(Theorem 2)\n");
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            printf("%7.2lf ",*(*(inmatrix+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n");
        E=mulMatrix(matrix,inmatrix,4);
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            printf("%7.2lf ",*(*(E+i)+j));
          }
          printf("\n");
        }
         printf("system> inverse of matrix A(Gaussian Elimination)\n");
        inmatrix_gaussian=inverseA_gaussian(matrix,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(inmatrix_gaussian+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n");
        E1=mulMatrix(matrix,inmatrix_gaussian,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(E1+i)+j));
          }
          printf("\n");
        }
        LU=matrix_LU(matrix,L,U,n);
		if(LU==NULL)
		{
			printf("system> Not Available (Matrix Triangularization)\n");
			continue;
		}
        printf("system> Lower triangular matrix\n");
	    
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(L+i)+j));
          }
          printf("\n");
        }
        printf("system> Upper triangular matrix\n");
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(U+i)+j));
          }
          printf("\n");
        }
        printf("system> L*U\n");
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(LU+i)+j));
          }
          printf("\n");
        }
      }
    }
    else if(n==5) //n이 5일 경우
    {
      inmatrix=inverseA55(matrix);
      if(detA55(matrix)==0)
      {
        printf("system> A is singular\n");
        continue;
      }
      else
      {
        printf("system> A is nonsingular\n");
        printf("system> inverse of matrix A(Theorem 2)\n");
		for(i=0;i<5;i++)
        {
          for(j=0;j<5;j++)
          {
             printf("%7.2lf",*(*(inmatrix+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n");
        E=mulMatrix(matrix,inmatrix,5);
        for(i=0;i<5;i++)
        {
          for(j=0;j<5;j++)
          {
            printf("%7.2lf",*(*(E+i)+j));
          }
          printf("\n");
        }
        printf("system> inverse of matrix A(Gaussian Elimination)\n");
        inmatrix_gaussian=inverseA_gaussian(matrix,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(inmatrix_gaussian+i)+j));
          }
          printf("\n");
        }
        printf("system> A*A-1\n");
        E1=mulMatrix(matrix,inmatrix_gaussian,n);
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(E1+i)+j));
          }
          printf("\n");
        }
        LU=matrix_LU(matrix,L,U,n);
		if(LU==NULL)
		{
			printf("system> Not Available (Matrix Triangularization)\n");
			continue;
		}	
        printf("system> Lower triangular matrix\n");
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(L+i)+j));
          }
          printf("\n");
        }
        printf("system> Upper triangular matrix\n");
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(U+i)+j));
          }
          printf("\n");
        }
        printf("system> L*U\n");
        for(i=0;i<n;i++)
        {
          for(j=0;j<n;j++)
          {
            printf("%7.2lf",*(*(LU+i)+j));
          }
          printf("\n");
        }
      }
    } 
  }
  for(i=0;i<n;i++)  //free 를 해준다.
  {
	free(*(matrix+i));
	free(*(inmatrix+i));
	free(*(L+i));
	free(*(U+i));
	free(*(LU+i));
	free(*(inmatrix_gaussian+i));
	free(*(E+i));
	free(*(E1+i));
  }
  free(matrix);
  free(inmatrix);
  free(L);
  free(U);
  free(LU);
  free(inmatrix_gaussian);
  free(E);
  free(E1);
  return 0;
}


/*
Name : addMatrix
Args : matrix A, matrix B, its size n*n
Ret  : pointer of matrix A+B of size n*n
*/
double** addMatrix(double** A, double** B, int n){ //A행렬과 B행렬을 더한 C행렬을 return 해주는 함수
  double **C; //A행렬과 B행렬의 합
  int i,j;
  C=(double **)malloc(sizeof(double *)*n);
  for(i=0;i<n;i++)
  {
    *(C+i)=(double *)malloc(sizeof(double)*n);
  }
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      *(*(C+i)+j)=*(*(A+i)+j)+*(*(B+i)+j);
    }
  }
  return C;
}
/*
Name : subMatrix
Args : matrix A,matrix B, its size n*n
Ret  : pointer of Matrix A-B of size n*n
*/
double** subMatrix(double** A, double** B, int n){ //A행렬과 B행렬을 뺸 C행렬을 return 해주는 함수
  double **C; //A행렬과 B행렬의 차
  int i,j;
  C=(double **)malloc(sizeof(double *)*n);
  for(i=0;i<n;i++)
  {
    *(C+i)=(double *)malloc(sizeof(double)*n);
  }
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      *(*(C+i)+j)=*(*(A+i)+j)-*(*(B+i)+j);
    }
  }

  return C;
}
/*
Name : mulMatrix
Args : matrix A,matrix B, its size n*n
Ret  : pointer of Matrix A*B of size n*n
*/
double** mulMatrix(double** A, double** B, int n){
  double **C; //A행렬과 B행렬의 차
  int i,j,k;
  C=(double **)malloc(sizeof(double *)*n);
  for(i=0;i<n;i++)
  {
    *(C+i)=(double *)malloc(sizeof(double)*n);
  }
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      for(k=0;k<n;k++)
      {
        *(*(C+i)+j)+=*(*(A+i)+k)**(*(B+k)+j);
      }
    }
  }

  return C;
}
/*
Name : detA
Args : matrix A, its size n*n
Ret  : det(1x1,2x2,3x3matrix)
*/
double detA(double **A, int n){  //matrix 가 2x2나 3x3일때 matrix 가 역행렬을 가지는지 안가지는 알려주는 함수
  double det; //판별자
  if(n==1)
  {
    det=(*(*(A+0)+0));
  } 
  else if(n==2)
  {
    det=*(*(A+0)+0)**(*(A+1)+1)-*(*(A+1)+0)**(*(A+0)+1);
  }
  else if(n==3)
  {  
    det=*(*(A+0)+0)*(*(*(A+1)+1)**(*(A+2)+2)-*(*(A+1)+2)**(*(A+2)+1))
      -*(*(A+0)+1)*(*(*(A+1)+0)**(*(A+2)+2)-*(*(A+1)+2)**(*(A+2)+0))
      +*(*(A+0)+2)*(*(*(A+1)+0)**(*(A+2)+1)-*(*(A+1)+1)**(*(A+2)+0)); 
  }

  if(det==0)
    return 0.0;
  else return det;
}
/*
Name : detA44
Args : matrix A its size n*n
Ret  : 0 or 1
*/
double detA44(double** A) //4x4행렬이 역행렬을 가지는지 안가지는지 판별해주는 함수
{
  double **B; //4x4 행렬 A의 역행렬 
  double **C; //4x4 행렬 A중 (1,1)~(2,2) 까지 저장해준 2x2행렬
  double **D; //4x4 행렬 B중 (3,3)~(4,4) 까지 저장해준 2x2 행렬 
  int i,j;
  B=(double **)malloc(sizeof(double *)*4);
  for(i=0;i<4;i++)
  {
    *(B+i)=(double *)malloc(sizeof(double *)*4);
  }
  B=inverseA(A,4);
  C=(double **)malloc(sizeof(double *)*2);
  for(i=0;i<2;i++)
  {
    *(C+i)=(double *)malloc(sizeof(double *)*2);
  }
  D=(double **)malloc(sizeof(double *)*2);
  for(i=0;i<2;i++)
  {
    *(D+i)=(double *)malloc(sizeof(double *)*2);
  }
  for(i=0;i<2;i++)
  {
    for(j=0;j<2;j++)
    {
      *(*(C+i)+j)=*(*(A+i)+j);
    }
  }
  for(i=0;i<2;i++)
  {
    for(j=0;j<2;j++)
    {
      *(*(D+i)+j)=*(*(B+i+2)+j+2);
    }
  }
  D=inverseA(D,2);
  for(i=0;i<4;i++)
	  free(*(B+i));
  free(B);
  if(detA(C,2)==0||detA(D,2)==0) return 0;
  else return 1;
}
/*
Name : detA4
Args : matrix A,its size 4x4
Ret  : det(4x4)
*/
double detA4(double **A) { //4x4 행렬의 det 값을 구해주는 함수
  int i,j;
  double det=0,*det3; //det 는 4x4행렬의 판별자,det3은 3x3 행렬의 판별자를 저장해준 변수
  det3=(double *)malloc(sizeof(double)*4);
  for(j=0;j<4;j++)
  {
    *(det3+j)=detA(getMijFromA(A,0,j,4),3);
    if((j+1)%2==1)
    {
      det+=*(*(A+0)+j)**(det3+j);
    }
    else
    {
      det+=(-1)**(*(A+0)+j)**(det3+j);
    }
  }
  
  return det;  
}
/*
Name : detA55
Args : matrix A its size 5*5
Ret  : det(5x5 matrix)
*/
double detA55(double **A){ //5x5행렬의 det값을 구해주는 함수
  int i,j;
  double det=0,*det4; //det4 는 m1j 의 det 이고 det는 5x5행렬의 det
  det4=(double *)malloc(sizeof(double)*5);
  for(j=0;j<5;j++)
  {
    *(det4+j)=detA4(getMijFromA(A,0,j,5));
    if((j+1)%2==1)
    {
      det+=*(*(A+0)+j)**(det4+j);
    }
    else
    {
      det+=(-1)**(*(A+0)+j)**(det4+j);
    } 
  }
    return det;
}
/*
Name : inverseA
Args : matrix Aits size n*n
Ret  : pointer of iA(inverse matrix)
*/
double** inverseA(double **A, int n){ //Theorem1을 이용해 역행렬을 구해주는 함수
  double **iA; //역행렬을 저장해주는 행렬
  int i,j; 
  double a,b,c,d,e,f,g,h,l; //3x3 matrix 의 역행렬을 구할때 역행렬의 각 요소를 구하는 것을 편하게 하기 위해 만든 변수들
  double **s1,**s2,**s3,**s4; // A행렬을 4등분한후 각각 2x2행렬인 s1,s2,s3,s4 에 넣어주기 위한 행렬들들
  double **is1,**is2,**is3,**is4;//s1,s2,s3,s4 의 역행렬
  double **p1,**p2,**p3,**p4; // A행렬의 역행렬을 4등분을 했을때 각 부분들을 담당하는 행렬들
  iA=(double **)malloc(sizeof(double *)*n);
  for(i=0;i<n;i++)
  {
    *(iA+i)=(double *)malloc(sizeof(double)*n);
  }
  if(n==1)
  {
    *(*(iA+0)+0)=1/(*(*(A+0)+0));
  }
  else if(n==2)
  {
    *(*(iA+0)+0)=1/detA(A,2)**(*(A+1)+1);
    *(*(iA+0)+1)=1/detA(A,2)**(*(A+0)+1)*(-1);
    *(*(iA+1)+0)=1/detA(A,2)**(*(A+1)+0)*(-1);
    *(*(iA+1)+1)=1/detA(A,2)**(*(A+0)+0);
  }
  else if(n==3)
  {

    a=(*(*(A+1)+1)**(*(A+2)+2)-*(*(A+1)+2)**(*(A+2)+1));
    b=(*(*(A+1)+0)**(*(A+2)+2)-*(*(A+1)+2)**(*(A+2)+0))*(-1);
    c=(*(*(A+1)+0)**(*(A+2)+1)-*(*(A+1)+1)**(*(A+2)+0));
    d=(*(*(A+0)+1)**(*(A+2)+2)-*(*(A+0)+2)**(*(A+2)+1))*(-1);
    e=(*(*(A+0)+0)**(*(A+2)+2)-*(*(A+0)+2)**(*(A+2)+0));
    f=(*(*(A+0)+0)**(*(A+2)+1)-*(*(A+0)+1)**(*(A+2)+0))*(-1);
    g=(*(*(A+0)+1)**(*(A+1)+2)-*(*(A+0)+2)**(*(A+1)+1));
    h=(*(*(A+0)+0)**(*(A+1)+2)-*(*(A+0)+2)**(*(A+1)+0))*(-1);
    l=(*(*(A+0)+0)**(*(A+1)+1)-*(*(A+0)+1)**(*(A+1)+0));

    *(*(iA+0)+0)=a/detA(A,3);
    *(*(iA+0)+1)=d/detA(A,3);
    *(*(iA+0)+2)=g/detA(A,3);
    *(*(iA+1)+0)=b/detA(A,3);
    *(*(iA+1)+1)=e/detA(A,3);
    *(*(iA+1)+2)=h/detA(A,3);
    *(*(iA+2)+0)=c/detA(A,3);
    *(*(iA+2)+1)=f/detA(A,3);
    *(*(iA+2)+2)=l/detA(A,3);
  }
  else if(n==4)
  {
    s1=(double **)malloc(sizeof(double *)*2);
    s2=(double **)malloc(sizeof(double *)*2);
    s3=(double **)malloc(sizeof(double *)*2);
    s4=(double **)malloc(sizeof(double *)*2);
    is1=(double **)malloc(sizeof(double *)*2);
    is2=(double **)malloc(sizeof(double *)*2);
    is3=(double **)malloc(sizeof(double *)*2);
    is4=(double **)malloc(sizeof(double *)*2);
    p1=(double **)malloc(sizeof(double *)*2);
    p2=(double **)malloc(sizeof(double *)*2);
    p3=(double **)malloc(sizeof(double *)*2);
    p4=(double **)malloc(sizeof(double *)*2); 
    for(i=0;i<2;i++)
    {
      *(p1+i)=(double *)malloc(sizeof(double)*2);
      *(p2+i)=(double *)malloc(sizeof(double)*2);
      *(p3+i)=(double *)malloc(sizeof(double)*2);
      *(p4+i)=(double *)malloc(sizeof(double)*2);
    }

    for(i=0;i<2;i++)
    {
      *(is1+i)=(double *)malloc(sizeof(double)*2);
      *(is2+i)=(double *)malloc(sizeof(double)*2);
      *(is3+i)=(double *)malloc(sizeof(double)*2);
      *(is4+i)=(double *)malloc(sizeof(double)*2);
    }
    for(i=0;i<2;i++)
    {
      *(s1+i)=(double *)malloc(sizeof(double)*2);
      *(s2+i)=(double *)malloc(sizeof(double)*2);
      *(s3+i)=(double *)malloc(sizeof(double)*2);
      *(s4+i)=(double *)malloc(sizeof(double)*2);
    }
    for(i=0;i<2;i++)
    {
      for(j=0;j<2;j++)
      {
        *(*(s1+i)+j)=*(*(A+i)+j);
        *(*(s2+i)+j)=*(*(A+i)+j+2);
        *(*(s3+i)+j)=*(*(A+i+2)+j);
        *(*(s4+i)+j)=*(*(A+i+2)+j+2);
      }
    }
    is1=inverseA(s1,2);
    is2=inverseA(s2,2);
    is3=inverseA(s3,2);
    is4=inverseA(s4,2);
    p4=mulMatrix(s3,is1,2);
    p4=mulMatrix(p4,s2,2);
    p4=subMatrix(s4,p4,2);
    p4=inverseA(p4,2);
    p3=mulMatrix(mulMatrix(p4,s3,2),is1,2);
    for(i=0;i<2;i++)
    {
      for(j=0;j<2;j++)
      {
        *(*(p3+i)+j)*=-1;
      }
    }
    p2=mulMatrix(mulMatrix(is1,s2,2),p4,2);
    for(i=0;i<2;i++)
    {
      for(j=0;j<2;j++)
      {
        *(*(p2+i)+j)*=-1;
      }
    }
    p1=addMatrix(is1,mulMatrix(mulMatrix(mulMatrix(mulMatrix(is1,s2,2),p4,2),s3,2),is1,2),2);
    for(i=0;i<2;i++) // iA(inverse matrix)의 4부분에 각 p1,p2,p3,p4 를 대입
    {
      for(j=0;j<2;j++)
      {
        *(*(iA+i)+j)=*(*(p1+i)+j);
        *(*(iA+i+2)+j)=*(*(p3+i)+j);  
        *(*(iA+i)+j+2)=*(*(p2+i)+j);  
		*(*(iA+i+2)+j+2)=*(*(p4+i)+j);
	  }
	}
	for(i=0;i<2;i++)
	{
		free(*(s1+i));
		free(*(s2+i));
		free(*(s3+i));
		free(*(s4+i));
		free(*(is1+i));
		free(*(is2+i));
		free(*(is3+i));
		free(*(is4+i));
		free(*(p1+i));
		free(*(p2+i));
		free(*(p3+i));
		free(*(p4+i));
	}
	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(is1);
	free(is2);
	free(is3);
	free(is4);
	free(p1);
	free(p2);
	free(p3);
	free(p4);
  }
  return iA;
}
/*
Name : inverseA55
Args : matrix A,its size 5*5
Ret  : iA(inverse Matrix) its size 5x5
*/
double** inverseA55(double** A){
  double **iA,**Cm;// Cm은 Cofactor Matrix  Mij 행렬은 i행과 j행을 제외한 4x4행렬
  int i,j;
  double det5;// 5x5행렬 A 의 판별자
  det5=detA55(A);
  iA=(double **)malloc(sizeof(double *)*5);
  Cm=(double **)malloc(sizeof(double *)*5);
  for(i=0;i<5;i++)
  {
    *(iA+i)=(double *)malloc(sizeof(double)*5);
    *(Cm+i)=(double *)malloc(sizeof(double)*5);
  }


  for(i=0;i<5;i++)
  {
   for(j=0;j<5;j++)
   {
    if((i+j+2)%2==0)
      *(*(Cm+i)+j)=detA4(getMijFromA(A,i,j,5));
    else
      *(*(Cm+i)+j)=detA4(getMijFromA(A,i,j,5))*(-1);
   }
  }
 Cm=transposeA(Cm,5);  //Cofactor를 전치시킨다((i,j) ->(j,i)로 바꿔준다)
 for(i=0;i<5;i++)
 {
  for(j=0;j<5;j++)
  { 
    *(*(iA+i)+j)=(*(*(Cm+i)+j))/det5;
   }
  }
   for(i=0;i<5;i++)
		free(*(Cm+i));
   free(Cm);
		
   return iA;
}

/*
Name : inverseA_gaussian
Args : matrix A its size n*n
Ret  : pointer of iA_gauss(gassian Elimination inverse matrix) of size n*n
*/
double** inverseA_gaussian(double **A, int n){ // gaussian Elimination 을 이용해서 역행렬을 구해주는 함수
   int i,j,x=0;
   double **E,**gauss,**iA_gauss; //iA_gauss  gauss 는 A를받아주는 행렬
   double temp,swap; //temp 는 1행 1열과 
   gauss=(double **)malloc(sizeof(double *)*n);
   E=(double **)malloc(sizeof(double *)*n);
   iA_gauss=(double **)malloc(sizeof(double *)*n);
   for(i=0;i<n;i++)
   {
     *(E+i)=(double *)malloc(sizeof(double)*n);
     *(gauss+i)=(double *)malloc(sizeof(double)*n);
     *(iA_gauss+i)=(double *)malloc(sizeof(double)*n);
   }
   for(i=0;i<n;i++) //E를 단위 행렬로 초기화
   {
     for(j=0;j<n;j++)
     {
       if(i==j) 
       {
         *(*(E+i)+j)=1;
       }
       else 
       {
         *(*(E+i)+j)=0; 
       }
     }
   }
   
   for(i=0;i<n;i++) // 
   {
     for(j=0;j<n;j++)
       *(*(gauss+i)+j)=*(*(A+i)+j); //gauss 를 A행렬로 초기화
   }
     for(x=0;x<n;x++) //gassian Elimination 사용
     {
       if(*(*(gauss+x)+x)==0)  // (x,x)행렬이 0일때 같은 열중 다른 행이 0이 아닐경우 두 행을 바꿔준다
       {
         for(i=x+1;i<n;i++)
         {
           if(*(*(gauss+i)+x)!=0) break;
         }
       for(j=0;j<n;j++)
       {
         swap=*(*(gauss+x)+j);
         *(*(gauss+x)+j)=*(*(gauss+i)+j);
         *(*(gauss+i)+j) =swap;
         swap=*(*(E+x)+j);
         *(*(E+x)+j)=*(*(E+i)+j);
         *(*(E+i)+j) =swap;
       }
       }
      temp=(*(*(gauss+x)+x));
     for(i=0;i<n;i++) 
     {
       *(*(gauss+x)+i)=*(*(gauss+x)+i)/temp;
       *(*(E+x)+i)=*(*(E+x)+i)/temp;
     }
     for(i=x+1;i<n;i++)
     {
       temp=*(*(gauss+i)+x)/(*(*(gauss+x)+x));
       for(j=0;j<n;j++)
       {
         *(*(gauss+i)+j)=*(*(gauss+i)+j)-*(*(gauss+x)+j)*temp;
         *(*(E+i)+j)=*(*(E+i)+j)-*(*(E+x)+j)*temp;
       } 
     }
    
   }
   for(x=n-1;x>=0;x--)
   {
      for(i=x-1;i>=0;i--)
      {
        temp=*(*(gauss+i)+x)/(*(*(gauss+x)+x));
        for(j=n-1;j>=0;j--)
        {
          *(*(gauss+i)+j)=*(*(gauss+i)+j)-*(*(gauss+x)+j)*temp;
          *(*(E+i)+j)=*(*(E+i)+j)-*(*(E+x)+j)*temp;
        }
      }
   }
   for(i=0;i<n;i++)
   {
     for(j=0;j<n;j++)
     {
       *(*(iA_gauss+i)+j)=*(*(E+i)+j);
     }
   }
	
   return iA_gauss;
}
/*
Name : matrix_LU
Args : matrix A,matrix L,matrix U, its size n*n
Ret  : pointer of result(L*U) of size n*n
*/
double** matrix_LU(double **A, double **L, double **U, int n){ //lower triangular matrix 와 Upper triangular matrix 와 두 matrix 의 곱을 구해주는 함수
	double **result,**E; // E는 단위행렬 result 는 L*U 행렬
	int i,j,x;
	double temp,swap; 
	E=(double **)malloc(sizeof(double *)*n);
	result=(double **)malloc(sizeof(double *)*n);
	for(i=0;i<n;i++)
	{
		*(E+i)=(double *)malloc(sizeof(double)*n);
		*(result+i)=(double *)malloc(sizeof(double)*n);
	}
	for(i=0;i<n;i++) //U 를 A로 초기화, L을 단위행렬로 초기화
	{
		for(j=0;j<n;j++)
		{
			if(i==j)
				*(*(L+i)+j)=1;
			else *(*(L+i)+j)=0;

			*(*(U+i)+j)=*(*(A+i)+j);
		}
	}
	if(*(*(U+0)+0)==0)
		return NULL;
	for(x=0;x<n;x++)
	{
		if(*(*(U+x)+x)==0)  // (x,x)가 0일때 행을 바꿔준다.
		{
			for(i=x+1;i<n;i++)
			{
				if(*(*(U+i)+x)!=0) break;
			}
			for(j=0;j<n;j++)
			{
				swap=*(*(U+x)+j);
				*(*(U+x)+j)=*(*(U+i)+j);
				*(*(U+i)+j) =swap;

			}
		} 
		for(i=x+1;i<n;i++)
		{
			temp=*(*(U+i)+x)/(*(*(U+x)+x));
			*(*(L+i)+x)=*(*(U+i)+x)/(*(*(U+x)+x));
			for(j=0;j<n;j++)
			{
				*(*(U+i)+j)=*(*(U+i)+j)-*(*(U+x)+j)*temp;
			} 
		}

	}  

	result=mulMatrix(L,U,n); //result 는 L*U
	return result;

}
/*
Name : transposeA
Args : matrix A, its size n*n
Ret  : pointer of result(transpose matrix) of size n*n
*/
double** transposeA(double** A, int n){  //(i,j)를 (j,i)로 바꿔주는 함수
   double **result;
  int i,j;
  result=(double **)malloc(sizeof(double*)*n);
  for(i=0;i<n;i++)
    *(result+i)=(double *)malloc(sizeof(double)*n);
  for(i=0;i<n;i++)
  {
   for(j=0;j<n;j++)
   {
      *(*(result+i)+j)=*(*(A+j)+i);
   }
  }
  return result; 
}
/*
Name : getMijFromA
Args : matrix A,i,j, its size n*n
Ret  : pointer result  of size n*n
*/
double** getMijFromA(double** A, int i, int j, int n){ // A행에 i행 j열 을 뺴주는 함수
  double **result;
  int x,y,x1=0,y1=0;
  result=(double **)malloc(sizeof(double*)*n-1);
  for(x=0;x<n-1;x++)
    *(result+x)=(double *)malloc(sizeof(double)*n-1);
  for(x=0;x<n;x++)
  {
    if(i==x) continue; 
    for(y=0;y<n;y++)
    {
      if(j==y) continue;
      {
        *(*(result+x1)+y1)=*(*(A+x)+y);
        y1++;
      }
    }
    x1++;
    y1=0;
  }
  return result;
}
