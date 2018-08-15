#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define swap(a,b,c) {c = a; a = b ; b = c; }

void   Mergesort(double *Y, unsigned int *P2, unsigned int *P3, unsigned int L, unsigned int R);
void   quicksort(double *A, unsigned int *P,  int L, int R);
void   partition(double *A, unsigned int *P, int L,int R,int* pivotpoint);



void   Mergesort(double *Y, unsigned int *P2, unsigned int *P3, unsigned int L, unsigned int R)
{
	/* �̹� THR���� ����
	���� ������ ��� Bubble Sort�� Y�������� ���־��� ������ �������� �ʰ� �ٷ� ���ؼ� �����ϸ� �ȴ�*/
	unsigned int mid, Lidx, Ridx, Pidx, i;
	mid = (L + R) / 2;
	Lidx = L;
	Ridx = mid + 1;
	Pidx = L;
	while (Lidx <= (R + L) / 2 && Ridx <= R)  //���� ������ ��Ʈ�� �� ���� �� ���ĵɶ� ����
	{
		if (Y[P2[Lidx]] < Y[P2[Ridx]]) //������Ʈ ���� <= ��������Ʈ ����
		{
			P3[Pidx] = P2[Lidx++];    //������Ʈ ���Ҹ� P3�� ����
		}
		else                          //������Ʈ ���� > ��������Ʈ ����
		{
			P3[Pidx] = P2[Ridx++];    //��������Ʈ ���Ҹ� P3�� ����
		}
		Pidx++;
	}
	if (Lidx > mid)		//������Ʈ ������ ���� ���� ���
	{
		for (i = Ridx; i <= R; i++)
		{
			P3[Pidx] = P2[i];
			Pidx++;
		}
	}
	else               //��������Ʈ ������ ���� ���� ���
	{
		for (i = Lidx; i <= mid; i++)
		{
			P3[Pidx] = P2[i];
			Pidx++;
		}
	}
	for (i = L; i <= R; i++)   //P3�����͸� P2�� �̵���Ų��.
		P2[i] = P3[i];
}

void   partition(double *A, unsigned int *P,int L,int R, int *pivotpoint)
{
	double pivotitem;
	int i, j;
	unsigned int temp;  //temp�� swap�ϰų� ���� �Ͻ������� �������ֱ� ���� ������� ����
	pivotitem = A[P[L]];   //partition

	j = L;
	for (i = L+1; i <= R; i++)
	{
		if (A[P[i]] < pivotitem)
		{
			j++;
			swap(P[i], P[j],temp);

		}
	}
	*pivotpoint = j;
	swap(P[L], P[*pivotpoint],temp);

}

void   quicksort(double *A, unsigned int *P,int L,int R)
{
	int pivotpoint;
	pivotpoint = L;
	if (R > L)
	{
		partition(A, P, L, R,&pivotpoint);
		if (L - (pivotpoint - 1) > pivotpoint + 1 - R)   /* L���� pivotpoint-1 ������ ���̿� pivotpoint+1 ���� R ������ ���̸� ���Ͽ� �� ª�� ������ ����
														 reculsive�ϰ� ���������μ� �ð��� �ٿ��ش�.*/
		{
			quicksort(A, P, L, pivotpoint - 1);
			quicksort(A, P, pivotpoint + 1, R);
		}
		else
		{
			quicksort(A, P, pivotpoint + 1, R);
			quicksort(A, P, L, pivotpoint - 1);
		}
	}
}
void   Sort_by_X_or_Y(double *A, unsigned int *P,unsigned int N)
{
	quicksort(A, P, 0, N-1);
}

double Closest_Pair_DC(
	unsigned int L, unsigned int R,		// left and right indices
	unsigned int *pt1, unsigned int *pt2,   // min dist points inices
	double *X, double *Y,					// input points position array
	unsigned int *P1, unsigned int *P2, unsigned int *P3, // temp index arrays
	unsigned int THR	// threshold value
	)
{
	unsigned int i, j,k;
	unsigned int pt1_temp, pt2_temp, temp;
	double d, dist, dr, dl;
	
	if (THR >= R - L + 1)		//�������� ������ THR���� ���� ���
	{
		for (i = L; i <= R; i++) //P1�迭�� ������� index�� P2�� ����
			P2[i] = P1[i];

		if (R - L + 1 == 2)  //�� ������ �ΰ��� index�� ���ؼ� P2�� ����
		{
			if (Y[P1[R]] < Y[P1[L]])
			{
				swap(P2[R], P2[L],temp);
			}			
			d = sqrt(pow(X[P2[R]] - X[P2[L]], 2) + pow(Y[P2[R]] - Y[P2[L]], 2)); //������ �Ÿ��� ���� d�� �����Ѵ�.
			*pt1 = P2[L];
			*pt2 = P2[R];
		}
		else if (R - L + 1 > 2)  //�� ������ �ΰ� �̻��̸� �ش������ P2 �迭�� ����� index�� bubble sort�� �����Ѵ�.
		{
			for (i = L; i <= R-1 ; i++)
			{
				for (j = i + 1; j <= R; j++)
				{
					if (Y[P2[i]] > Y[P2[j]])
						swap(P2[i], P2[j],temp);
				}
			}
		}

		d = 99999999999;
		for (i = L; i <= R - 1; i++) //Brute force�� ������� closest pair�� ���� 
		{
			for (j = i + 1; j <= R; j++)
			{
				if (fabs(Y[P2[j]] - Y[P2[i]]) >= d) break;
				dist = sqrt(pow(X[P2[i]] - X[P2[j]], 2) + pow(Y[P2[i]] - Y[P2[j]], 2));
				if (dist < d)
				{
					d = dist;
					*pt1 = P2[i];  //distance�� �ּ��ε����� Y�迭������ index
					*pt2 = P2[j];
				}
			}
		}
		return d;
	}
	else          //�������� ������ THR���� Ŭ ��� 
	{
		dl = Closest_Pair_DC(L, (L + R) / 2, pt1, pt2, X, Y, P1, P2, P3, THR);
		pt1_temp = *pt1;
		pt2_temp = *pt2;

		dr = Closest_Pair_DC((L + R) / 2 + 1, R, pt1, pt2, X, Y, P1, P2, P3, THR);

		if (dl >= dr)  //dl�� dr ���� Ŭ ��� dr=d �� �ȴ�.
			d = dr;
		else         //dr�� dl���� Ŭ ��� dl=d �� �ǰ�	pt1,2�� dl������ �Ÿ��� �ּ��� ������ index�� �Ǿ���Ѵ�.
		{
			d = dl;
			*pt1 = pt1_temp;
			*pt2 = pt2_temp;
		}
		Mergesort(Y, P2, P3, L, R);
		
		/* �߾Ӽ����� ���� d�Ÿ��� �ִ� ������ P3�� ���� */
		k = 0;
		for (i = L; i <= R; i++)
		{
			if (X[P2[i]] >= X[P1[(L + R) / 2]] - d && X[P2[i]] <= X[P1[(L + R) / 2]] + d)
			{
				P3[k] = P2[i];
				k++;
			}
		}

		/*�߾Ӽ��������� d�Ÿ��� �ִ� ������ �Ÿ��� ���ϰ� �� �Ÿ��� d���� ª�ٸ� d�� �����ϰ� �׶��� �ε����� pt1 pt2�� �����Ѵ� */
		for (i = 0; i < k; i++)
		{
			for (j = i + 1; j < k && j <= i + 15; j++)
			{
				if (fabs(Y[P3[j]] - Y[P3[i]]) >= d)
					break;
				dist = sqrt(pow(X[P3[i]] - X[P3[j]], 2) + pow(Y[P3[i]] - Y[P3[j]], 2));
				if (dist < d)
				{
					d = dist;
					*pt1 = P3[i];
					*pt2 = P3[j];
				}
			}
		}
		return d;
	}

}