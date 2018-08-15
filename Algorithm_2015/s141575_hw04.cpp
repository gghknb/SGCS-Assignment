#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define swap(a,b,c) {c = a; a = b ; b = c; }

void   Mergesort(double *Y, unsigned int *P2, unsigned int *P3, unsigned int L, unsigned int R);
void   quicksort(double *A, unsigned int *P,  int L, int R);
void   partition(double *A, unsigned int *P, int L,int R,int* pivotpoint);



void   Mergesort(double *Y, unsigned int *P2, unsigned int *P3, unsigned int L, unsigned int R)
{
	/* 이미 THR보다 적은
	점의 개수일 경우 Bubble Sort로 Y축정렬을 해주었기 때문에 분할하지 않고 바로 비교해서 병합하면 된다*/
	unsigned int mid, Lidx, Ridx, Pidx, i;
	mid = (L + R) / 2;
	Lidx = L;
	Ridx = mid + 1;
	Pidx = L;
	while (Lidx <= (R + L) / 2 && Ridx <= R)  //왼쪽 오른쪽 파트중 한 쪽이 다 정렬될때 까지
	{
		if (Y[P2[Lidx]] < Y[P2[Ridx]]) //왼쪽파트 원소 <= 오른쪽파트 원소
		{
			P3[Pidx] = P2[Lidx++];    //왼쪽파트 원소를 P3에 삽입
		}
		else                          //왼쪽파트 원소 > 오른쪽파트 원소
		{
			P3[Pidx] = P2[Ridx++];    //오른쪽파트 원소를 P3에 삽입
		}
		Pidx++;
	}
	if (Lidx > mid)		//왼쪽파트 정렬이 먼저 끝난 경우
	{
		for (i = Ridx; i <= R; i++)
		{
			P3[Pidx] = P2[i];
			Pidx++;
		}
	}
	else               //오른쪽파트 정렬이 먼저 끝난 경우
	{
		for (i = Lidx; i <= mid; i++)
		{
			P3[Pidx] = P2[i];
			Pidx++;
		}
	}
	for (i = L; i <= R; i++)   //P3데이터를 P2에 이동시킨다.
		P2[i] = P3[i];
}

void   partition(double *A, unsigned int *P,int L,int R, int *pivotpoint)
{
	double pivotitem;
	int i, j;
	unsigned int temp;  //temp는 swap하거나 값을 일시적으로 저장해주기 위해 사용해준 변수
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
		if (L - (pivotpoint - 1) > pivotpoint + 1 - R)   /* L부터 pivotpoint-1 까지의 길이와 pivotpoint+1 부터 R 까지의 길이를 비교하여 더 짧은 구간을 먼저
														 reculsive하게 돌려줌으로서 시간을 줄여준다.*/
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
	
	if (THR >= R - L + 1)		//현재점의 개수가 THR보다 작을 경우
	{
		for (i = L; i <= R; i++) //P1배열의 대상점의 index를 P2에 복사
			P2[i] = P1[i];

		if (R - L + 1 == 2)  //점 개수가 두개면 index를 비교해서 P2에 저장
		{
			if (Y[P1[R]] < Y[P1[L]])
			{
				swap(P2[R], P2[L],temp);
			}			
			d = sqrt(pow(X[P2[R]] - X[P2[L]], 2) + pow(Y[P2[R]] - Y[P2[L]], 2)); //두점의 거리를 구해 d에 저장한다.
			*pt1 = P2[L];
			*pt2 = P2[R];
		}
		else if (R - L + 1 > 2)  //점 개수가 두개 이상이면 해당범위의 P2 배열에 저장된 index를 bubble sort로 정렬한다.
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
		for (i = L; i <= R - 1; i++) //Brute force한 방법으로 closest pair을 구함 
		{
			for (j = i + 1; j <= R; j++)
			{
				if (fabs(Y[P2[j]] - Y[P2[i]]) >= d) break;
				dist = sqrt(pow(X[P2[i]] - X[P2[j]], 2) + pow(Y[P2[i]] - Y[P2[j]], 2));
				if (dist < d)
				{
					d = dist;
					*pt1 = P2[i];  //distance가 최소인두점의 Y배열에서의 index
					*pt2 = P2[j];
				}
			}
		}
		return d;
	}
	else          //현재점의 개수가 THR보다 클 경우 
	{
		dl = Closest_Pair_DC(L, (L + R) / 2, pt1, pt2, X, Y, P1, P2, P3, THR);
		pt1_temp = *pt1;
		pt2_temp = *pt2;

		dr = Closest_Pair_DC((L + R) / 2 + 1, R, pt1, pt2, X, Y, P1, P2, P3, THR);

		if (dl >= dr)  //dl이 dr 보다 클 경우 dr=d 가 된다.
			d = dr;
		else         //dr이 dl보다 클 경우 dl=d 가 되고	pt1,2이 dl에서의 거리가 최소인 두점의 index가 되어야한다.
		{
			d = dl;
			*pt1 = pt1_temp;
			*pt2 = pt2_temp;
		}
		Mergesort(Y, P2, P3, L, R);
		
		/* 중앙선에서 부터 d거리에 있는 점들을 P3에 저장 */
		k = 0;
		for (i = L; i <= R; i++)
		{
			if (X[P2[i]] >= X[P1[(L + R) / 2]] - d && X[P2[i]] <= X[P1[(L + R) / 2]] + d)
			{
				P3[k] = P2[i];
				k++;
			}
		}

		/*중앙선에서부터 d거리에 있는 점들의 거리를 구하고 그 거리가 d보다 짧다면 d를 갱신하고 그때의 인덱스를 pt1 pt2에 저장한다 */
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