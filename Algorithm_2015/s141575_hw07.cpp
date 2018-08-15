int Promising(int N, int *Color, char **Adj_M, int nowvertex, int usecolor);
int QuickSort(int *P1, int *P2, int low, int high);
int m_coloring(int N, int m, int *Color, int *P2, char **Adj_M, int nowvertex);
int Graph_mColoring_top(
	int N,			// vertex의 개수
	int m,			// 허용된 칼라수
	int *Color,		// 배정된 칼라 값 (꼭 0, 1, 2, 등 순서대로 일 필요는 없음)
					// 즉, 2, 5, 8 등 연속이 아니어도 무방함.
	int *P1, int *P2, int *P3,	// 크기가 N인 임시 정수 배열
	char **Adj_M	// adjacency matrix (정수 1 또는 0이 저장)
)
{
	int i, j;
	int maxcolor;
	for(i = 0; i < N; i++)
	{
		P1[i] = 0;  //각 vertex에 저장된 degree개수
		P2[i] = i;  //degree개수 순서대로 정렬된 index
		Color[i] = 0;
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(Adj_M[i][j] == 1)
			{
				P1[i] = P1[i] + 1;	//P1 초기화
			}
		}
	}
	QuickSort(P1,P2,0,N-1);	//index quicksort로 P2정렬
	Color[P2[0]] = 1;
	maxcolor = m_coloring(N,m,Color,P2,Adj_M,1);	//첫번째 vertex에서부터 coloring
	if(maxcolor != 0)	//coloring이 제대로 되었을 경우
	{
		for(i = 0; i < N; i++)
		{
			Color[i] -= 1;
		}
	}
	return maxcolor;
}

/* vertex에서 색이 잘 배치되었는지 확인 */
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
	if(nowvertex == N)  //N=nowvertex일경우(모든 vertex들의 색 배치가 끝났을경우) 가장 큰 Color값을 return
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
	else if(nowvertex < N)	//N보다 작은 경우 
	{
		for(i = 1; i <= m; i++)
		{
			flag = Promising(N,Color,Adj_M,P2[nowvertex],i);	 //nowvertex에 색깔i의 배치가 맞는지 확인
			if(flag == 1)
			{
				Color[P2[nowvertex]] = i;
				maxcolor = m_coloring(N,m,Color,P2,Adj_M,nowvertex+1); //reculsive로 나머지 vertex들에게 색을 배치 
				if(maxcolor != 0)
				{
					return maxcolor;
				}
				Color[P2[nowvertex]] = 0; //nowvertex에서 color 배치가 맞지 않다면 제거.
			}
		}
	}
	return 0;
}