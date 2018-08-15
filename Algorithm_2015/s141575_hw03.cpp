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
	int pivotitem, pivotpoint, i, j, temp;//temp�� swap�ϰų� ���� �Ͻ������� �������ֱ� ���� ������� ����

	for (i = L; i <R; i++)   //����� �̹� ���ĵǾ� �ִ� ��� quick sort�� ��������ش�.
	{
		if (A[Aidx[i]] > A[Aidx[i + 1]])  
			break;
	}
	if (i == R) return;     //i�� R�� �ɶ� ���� ���� for���� break�� ���� �ʾ��� ��� A����� ���ĵǾ����� return�� ���ش�.

	if (R - L <= 200)       //����� ���̰� 200������ ��쿡 insertion sort�� �������ش�.
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
	else                      //����� ���̰� 200���� Ŭ ��� quick sort�� �̿��Ͽ� �����Ͽ� �ش�.
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

		if (L - (pivotpoint - 1) > pivotpoint + 1 - R)   /* L���� pivotpoint-1 ������ ���̿� pivotpoint+1 ���� R ������ ���̸� ���Ͽ� �� ª�� ������ ���� 
														 reculsive�ϰ� ���������μ� �ð��� �ٿ��ش�.*/
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
