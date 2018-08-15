#include <stdio.h>
#include <stdlib.h>
#define swap(a,b,c) {c = a; a = b ; b = c; }

void Error_Exit(char *s);

void Index_Sort_Quick(
	// sort Aidx[] such that A[Aidx[i]], i = 0, 1, ..., N-1, are non-decreasing order
	int *A,		// array of keys
	int *Aidx,  // index array of A (initially 0, 1, ..., N-1)
	int L,		// lestmost index
	int R		// rightmost index
	) {
	int pivotitem, pivotpoint, i, j, temp;//temp는 swap하거나 값을 일시적으로 저장해주기 위해 사용해준 변수

	for (i = L; i <R; i++)   //행렬이 이미 정렬되어 있는 경우 quick sort를 종료시켜준다.
	{
		if (A[Aidx[i]] > A[Aidx[i + 1]])  
			break;
	}
	if (i == R) return;     //i가 R이 될때 까지 위의 for문이 break가 되지 않았을 경우 A행렬이 정렬되었으니 return을 해준다.

	if (R - L <= 200)       //행렬의 길이가 200이하일 경우에 insertion sort로 정렬해준다.
	{
		for (i = L + 1; i <= R; i++)   //insertion sort
		{
			pivotitem = A[Aidx[i]];
			temp = Aidx[i];
			for (j = i - 1; j >= L && A[Aidx[j]] > pivotitem; j--)
			{
				Aidx[j + 1] = Aidx[j];
			}
			Aidx[j + 1] = temp;
		}
	}
	else                      //행렬의 길이가 200보다 클 경우 quick sort를 이용하여 정렬하여 준다.
	{
		pivotitem = A[Aidx[L]];   //partition
		j = L;
		for (i = L + 1; i <= R; i++)
		{
			if (A[Aidx[i]] < pivotitem)
			{
				j++;
				swap(Aidx[i], Aidx[j], temp);
			}
		}
		pivotpoint = j;
		swap(Aidx[L], Aidx[pivotpoint], temp);

		if (L - (pivotpoint - 1) > pivotpoint + 1 - R)   /* L부터 pivotpoint-1 까지의 길이와 pivotpoint+1 부터 R 까지의 길이를 비교하여 더 짧은 구간을 먼저 
														 reculsive하게 돌려줌으로서 시간을 줄여준다.*/
		{
			Index_Sort_Quick(A, Aidx, L, pivotpoint - 1);
			Index_Sort_Quick(A, Aidx, pivotpoint + 1, R);
		}
		else
		{
			Index_Sort_Quick(A, Aidx, pivotpoint + 1, R);
			Index_Sort_Quick(A, Aidx, L, pivotpoint - 1);
		}
	}
}
