#include <stdio.h>
#include <string.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
void Edit_Distance(
	// inputs
	char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,
	// insertion, deletion, substitution cost
	// outputs
	int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
	char **SR,		// Source string with insertion to T marks '*'
	char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
	char **TR,       // Edited string with deletion form S mark '*'
	// NOTE: ST, OP, TR must be allocated inside the function as needed(minimum as possible)
	int  *Mem_Allocated
	)
{
	int i, j;
	int Slen, Tlen;
	int Eij;
	int sbcost;
	int lencount,count;
	Slen = strlen(SS);
	Tlen = strlen(TS);

	/* Table initialization */
	for (i = 0; i <= Slen; i++)
		Table[i][0] = i*del_cost;
	for (j = 0; j <= Tlen; j++)
		Table[0][j] = j*ins_cost;

	/*edit distance를 계산하여 Table에 넣음*/
	for (i = 1; i <= Slen; i++)
	{
		for (j = 1; j <= Tlen; j++)
		{
			if (SS[i-1] == TS[j-1])
				sbcost = 0;
			else sbcost = sub_cost;
			Eij = min(Table[i - 1][j - 1] + sbcost, Table[i - 1][j] + del_cost);
			Eij = min(Eij, Table[i][j - 1] + ins_cost);
			Table[i][j] = Eij;
		}
	}
	

	i = Slen;
	j = Tlen;
	/*OP SR TR의 길이를 구하기 위해 Backtracking 하며 count 해준다.*/
	lencount = 0;
	while (1)
	{
		/* i와 j가 모두 0일 경우 Backtracking 종료*/
		if (i == 0 && j == 0)
			break;
		if (i == 0) //i가 0일 경우 insert만 해준다.
		{
			j--;
			lencount++;
			continue;
		}
		if (j == 0) //j가 0일 경우 delete만 해준다.
		{
			i--;
			lencount++;
			continue;
		}

		/* i나 j가 0일 경우*/
		
		if (Table[i][j - 1] + ins_cost == Table[i][j])  // (i,j-1)에서 insert한 cost가 (i,j)의 cost와 같을 경우
		{
			if (Table[i - 1][j] + del_cost == Table[i][j])  //parent 선택이 둘(insert,delete) 일 경우 
			{
				if (ins_cost <= del_cost)  //ins_cost <= del_cost 일경우 insert를 선택
				{
					lencount++;
					j--;
					continue;
				}
				if (ins_cost > del_cost)   //ins_cost>del_cost일 경우 delete 를 선택
				{
					lencount++;
					i--;
					continue;
				}
			}
			else //parent의 선택이 insert만일 경우
			{
				lencount++;
				j--;
				continue;
			}
		}

		if (Table[i - 1][j - 1] == Table[i][j])		//S의 i 인덱스와 T의 j인덱스가 같을경우 subcost를 0으로둔다.
			sbcost = 0;
		else sbcost = sub_cost;  //다른경우 주어진 sbcost = sub_cost

		if (Table[i - 1][j] + del_cost == Table[i][j])  //parent의 선태이 delete일 경우
		{
			lencount++;
			i--;
			continue;
		}

		if (Table[i - 1][j - 1] + sbcost == Table[i][j])  //parent의 선택이 insert,delete 둘다 아니고 substitute 일 경우
		{
			lencount++;
			i--;
			j--;
			continue;
		}

	}
		
	/* SR OP TR Allocation */
	*SR = new char[lencount + 1];
	*OP = new char[lencount + 1];
	*TR = new char[lencount + 1];
	i = Slen;
	j = Tlen;
	count = lencount--;
	
	/*SR OP TR에 맞는 문자열들을 삽입*/
	while (1)
	{
		if (i == 0	&& j == 0)
			break;
		if (i == 0 && j != 0) //i가 0일 경우 insert만 해준다.(SR = *,OP = i,T = TS[j-1])
		{
			(*SR)[lencount] = '*';
			(*OP)[lencount] = 'i';
			(*TR)[lencount] = TS[j - 1];
			j--;
			lencount--;
			continue;
		}
		if (j == 0 && i != 0) //j가 0일 경우 delete만 해준다.(SR= SS[i-1],OP = d,T= * )
		{
			(*SR)[lencount] = SS[i - 1];
			(*OP)[lencount] = 'd';
			(*TR)[lencount] = '*';
			i--;
			lencount--;
			continue;
		}
		
		/* i나 j가 0일 경우*/
		
		if (Table[i][j - 1] + ins_cost == Table[i][j])  // (i,j-1)에서 insert한 cost가 (i,j)의 cost와 같을 경우
		{
			if (Table[i - 1][j] + del_cost == Table[i][j])  //parent 선택이 둘(insert,delete) 일 경우 
			{
				if (ins_cost <= del_cost)  //ins_cost <= del_cost 일경우 insert를 선택(SR = *,OP = i,T = TS[j-1])
				{
					(*SR)[lencount] = '*';
					(*OP)[lencount] = 'i';
					(*TR)[lencount] = TS[j-1];
					j--;
					lencount--;
					continue;
				}
				if (ins_cost > del_cost)   //ins_cost>del_cost일 경우 delete 를 선택(SR= SS[i-1],OP = d,T= * )
				{
					(*SR)[lencount] = SS[i-1];
					(*OP)[lencount] = 'd';
					(*TR)[lencount] = '*';
					i--;
					lencount--;
					continue;
				}
			}
			else //parent의 선택이 insert만일 경우(SR = *,OP = i,T = TS[j-1])
			{
				(*SR)[lencount] = '*';
				(*OP)[lencount] = 'i';
				(*TR)[lencount] = TS[j-1];
				j--;
				lencount--;
				continue;
			}
		}

		if (Table[i - 1][j] + del_cost == Table[i][j])  //parent의 선택이 delete일 경우(SR= SS[i-1],OP = d,T= * )
		{
			(*SR)[lencount] = SS[i-1];
			(*OP)[lencount] = 'd';
			(*TR)[lencount] = '*';
			i--;
			lencount--;
			continue;
		}
		
		if (Table[i - 1][j - 1] == Table[i][j])		//S의 i 인덱스와 T의 j인덱스가 같을경우 subcost를 0으로둔다.
			sbcost = 0;
		else sbcost = sub_cost;  //다른경우 주어진 sbcost = sub_cost

		if (sbcost == 0)  //sbcost == 0 일 경우(OP='.',SR=SS[i-1],TR=TS[j-1])
		{
			if (Table[i - 1][j - 1] + sbcost == Table[i][j])
			{
				(*SR)[lencount] = SS[i-1];
				(*OP)[lencount] = '.';
				(*TR)[lencount] = TS[j-1];
				i--;
				j--;
				lencount--;
				continue;
			}
		}
		else    //sbcost == sub_cost 일 경우.
		{
			if (Table[i - 1][j - 1] + sbcost == Table[i][j])  //parent의 선택이 insert,delete 둘다 아니고 substitute 일 경우(OP='s',SR=SS[i-1],TR=TS[j-1])
			{
				(*SR)[lencount] = SS[i-1];
				(*OP)[lencount] = 's';
				(*TR)[lencount] = TS[j-1];
				i--;
				j--;
				lencount--;
				continue;
			}
		}
	}

	/* SR,OP,TR이 string으로 인식되려면 마지막에 NULL POINTER를 삽입 해줘야 함*/
	(*SR)[count] = '\0';
	(*OP)[count] = '\0';
	(*TR)[count] = '\0';
	
	*Mem_Allocated =(int)(strlen(*SR)+ strlen(*TR) + strlen(*OP)) + 3;

}