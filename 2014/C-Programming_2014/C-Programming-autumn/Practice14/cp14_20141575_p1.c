#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef float* Vector_Components;
typedef float Component;

typedef double VectorSize;
typedef double Distance;
typedef float Scalar;
typedef double Angle;

typedef struct{
	Vector_Components comps;
	int vec_size;
}Vector;

VectorSize getsize(Vector v);
Distance getDistance(Vector v1,Vector v2);
Scalar getDotProduct(Vector v1,Vector v2);
Angle getAngle(Vector v1,Vector v2);

int main()
{
	int i;
	VectorSize size1,size2;
	Vector v1,v2;
	Distance dis;
	Scalar dot;
	Angle ang;	
	
	/* Input Vector 1 */
	printf("Input size of vector v1 : ");
	scanf("%d",&v1.vec_size);
	
	v1.comps=(Vector_Components)malloc(sizeof(Component)*v1.vec_size);
	
	printf("Input %d components of v1 : ",v1.vec_size);
	for(i=0;i<v1.vec_size;i++)
		scanf("%f",v1.comps+i);	
	
	/* Input Vector 2 */
	printf("Input size of vector v2 : ");
	scanf("%d",&v2.vec_size);
	
	v2.comps=(Vector_Components)malloc(sizeof(Component)*v2.vec_size);
	
	printf("Input %d components of v2 : ",v2.vec_size);
	for(i=0;i<v2.vec_size;i++)
		scanf("%f",v2.comps+i);
	
	/* Output */
	printf("Result\n");
	
	size1=getsize(v1);
	printf("size of v1 = %.2f\n",size1);
	
	size2=getsize(v2);
	printf("size of v2 = %.2f\n",size2);
	
	if(v1.vec_size!=v2.vec_size)
		return 0;
	else
	{
		dis=getDistance(v1,v2);
		printf("Distance between v1 and v2 = %.3lf\n",dis);
		
		dot=getDotProduct(v1,v2);
		printf("Dot product v1 * v2 = %.0f\n",dot);
		
		ang=getAngle(v1,v2);
		printf("Angle between v1 and v2 = %.2lf\n",ang);
	}
	return 0;
}

VectorSize getsize(Vector v)
{
	int i;
	VectorSize size = 0;
	for(i=0;i<v.vec_size;i++)
		size+=*(v.comps+i)**(v.comps+i);
	size=sqrt(size);
	return size;
}
Distance getDistance(Vector v1,Vector v2)
{
	Distance dist = 0;
	int i;
	for(i=0;i<v1.vec_size;i++)
		dist+=(*(v1.comps+i)-*(v2.comps+i))*(*(v1.comps+i)-*(v2.comps+i));
	dist=fabs(dist);
	dist=sqrt(dist);
	return dist;
}

Scalar getDotProduct(Vector v1,Vector v2)
{
	int i;
	Scalar dot = 0;
	for(i=0;i<v1.vec_size;i++)
		dot+=*(v1.comps+i)**(v2.comps+i);
	return dot;
}

Angle getAngle(Vector v1,Vector v2)
{
	Angle ang; // double
	Scalar cos;  // float
	Angle cos2;
	Scalar dot;
	VectorSize size1,size2; // double
	int i;
	
	cos = getDotProduct(v1, v2) / (getsize(v1) * getsize(v2));
	cos2 = getDotProduct(v1, v2) / (getsize(v1) * getsize(v2));
	ang = acos(cos2) * 180/M_PI;
	return ang;
}
