int Promising(int N, int *Color, char **Adj_M, int nowvertex, int usecolor);
int QuickSort(int *P1, int *P2, int low, int high);
int m_coloring(int N, int m, int *Color, int *P2, char **Adj_M, int nowvertex);
int Graph_mColoring_top(
	int N,			// vertex�� ����
	int m,			// ���� Į���
	int *Color,		// ������ Į�� �� (�� 0, 1, 2, �� ������� �� �ʿ�� ����)
					// ��, 2, 5, 8 �� ������ �ƴϾ ������.
	int *P1, int *P2, int *P3,	// ũ�Ⱑ N�� �ӽ� ���� �迭
	char **Adj_M	// adjacency matrix (���� 1 �Ǵ� 0�� ����)
)
{
	int i, j;
	int maxcolor;
	for(i = 0; i < N; i++)
	{
		P1[i] = 0;  //�� vertex�� ����� degree����
		P2[i] = i;  //degree���� ������� ���ĵ� index
		Color[i] = 0;
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(Adj_M[i][j] == 1)
			{
				P1[i] = P1[i] + 1;	//P1 �ʱ�ȭ
			}
		}
	}
	QuickSort(P1,P2,0,N-1);	//index quicksort�� P2����
	Color[P2[0]] = 1;
	maxcolor = m_coloring(N,m,Color,P2,Adj_M,1);	//ù��° vertex�������� coloring
	if(maxcolor != 0)	//coloring�� ����� �Ǿ��� ���
	{
		for(i = 0; i < N; i++)
		{
			Color[i] -= 1;
		}
	}
	return maxcolor;
}

/* vertex���� ���� �� ��ġ�Ǿ����� Ȯ�� */
int Promising(int N, int *Color, char **Adj_M, int nowvertex, int usecolor)
{
	int i;
	int flag;
	flag = 1;
	for(i = 0; i < N; i++)
	{
		if(Adj_M[nowvertex][i] && Color[i] == usecolor)
		{
			flag = 0;
			break;
		}
	}
	return flag;
}
int QuickSort(int *P1, int *P2, int low, int high)
{
	int i, j;
	int pivotitem, temp;
	pivotitem = P1[P2[low]];
	j = low;
	for(i = low + 1; i <= high; i++)
	{
		if(P1[P2[i]] > pivotitem)
		{
			j = j + 1;
			temp = P2[i];
			P2[i] = P2[j];
			P2[j] = temp;
		}
	}
	temp = P2[low];
	P2[low] = P2[j];
	P2[j] = temp;
	if(low < j - 1)
	{
		QuickSort(P1,P2,low,j-1);
	}
	if(j + 1 < high)
	{
		QuickSort(P1,P2,j+1,high);
	}
	return 0;
}

int m_coloring(int N, int m, int *Color, int *P2, char **Adj_M, int nowvertex)
{
	int i;
	int maxcolor, flag;
	if(nowvertex == N)  //N=nowvertex�ϰ��(��� vertex���� �� ��ġ�� ���������) ���� ū Color���� return
	{
		maxcolor = Color[0];
		for(i = 1; i < N; i++)
		{
			if(Color[i] > maxcolor)
			{
				maxcolor = Color[i];
			}
		}
		return maxcolor;
	}
	else if(nowvertex < N)	//N���� ���� ��� 
	{
		for(i = 1; i <= m; i++)
		{
			flag = Promising(N,Color,Adj_M,P2[nowvertex],i);	 //nowvertex�� ����i�� ��ġ�� �´��� Ȯ��
			if(flag == 1)
			{
				Color[P2[nowvertex]] = i;
				maxcolor = m_coloring(N,m,Color,P2,Adj_M,nowvertex+1); //reculsive�� ������ vertex�鿡�� ���� ��ġ 
				if(maxcolor != 0)
				{
					return maxcolor;
				}
				Color[P2[nowvertex]] = 0; //nowvertex���� color ��ġ�� ���� �ʴٸ� ����.
			}
		}
	}
	return 0;
}