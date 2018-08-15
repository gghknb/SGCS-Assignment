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

	/*edit distance�� ����Ͽ� Table�� ����*/
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
	/*OP SR TR�� ���̸� ���ϱ� ���� Backtracking �ϸ� count ���ش�.*/
	lencount = 0;
	while (1)
	{
		/* i�� j�� ��� 0�� ��� Backtracking ����*/
		if (i == 0 && j == 0)
			break;
		if (i == 0) //i�� 0�� ��� insert�� ���ش�.
		{
			j--;
			lencount++;
			continue;
		}
		if (j == 0) //j�� 0�� ��� delete�� ���ش�.
		{
			i--;
			lencount++;
			continue;
		}

		/* i�� j�� 0�� ���*/
		
		if (Table[i][j - 1] + ins_cost == Table[i][j])  // (i,j-1)���� insert�� cost�� (i,j)�� cost�� ���� ���
		{
			if (Table[i - 1][j] + del_cost == Table[i][j])  //parent ������ ��(insert,delete) �� ��� 
			{
				if (ins_cost <= del_cost)  //ins_cost <= del_cost �ϰ�� insert�� ����
				{
					lencount++;
					j--;
					continue;
				}
				if (ins_cost > del_cost)   //ins_cost>del_cost�� ��� delete �� ����
				{
					lencount++;
					i--;
					continue;
				}
			}
			else //parent�� ������ insert���� ���
			{
				lencount++;
				j--;
				continue;
			}
		}

		if (Table[i - 1][j - 1] == Table[i][j])		//S�� i �ε����� T�� j�ε����� ������� subcost�� 0���εд�.
			sbcost = 0;
		else sbcost = sub_cost;  //�ٸ���� �־��� sbcost = sub_cost

		if (Table[i - 1][j] + del_cost == Table[i][j])  //parent�� ������ delete�� ���
		{
			lencount++;
			i--;
			continue;
		}

		if (Table[i - 1][j - 1] + sbcost == Table[i][j])  //parent�� ������ insert,delete �Ѵ� �ƴϰ� substitute �� ���
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
	
	/*SR OP TR�� �´� ���ڿ����� ����*/
	while (1)
	{
		if (i == 0	&& j == 0)
			break;
		if (i == 0 && j != 0) //i�� 0�� ��� insert�� ���ش�.(SR = *,OP = i,T = TS[j-1])
		{
			(*SR)[lencount] = '*';
			(*OP)[lencount] = 'i';
			(*TR)[lencount] = TS[j - 1];
			j--;
			lencount--;
			continue;
		}
		if (j == 0 && i != 0) //j�� 0�� ��� delete�� ���ش�.(SR= SS[i-1],OP = d,T= * )
		{
			(*SR)[lencount] = SS[i - 1];
			(*OP)[lencount] = 'd';
			(*TR)[lencount] = '*';
			i--;
			lencount--;
			continue;
		}
		
		/* i�� j�� 0�� ���*/
		
		if (Table[i][j - 1] + ins_cost == Table[i][j])  // (i,j-1)���� insert�� cost�� (i,j)�� cost�� ���� ���
		{
			if (Table[i - 1][j] + del_cost == Table[i][j])  //parent ������ ��(insert,delete) �� ��� 
			{
				if (ins_cost <= del_cost)  //ins_cost <= del_cost �ϰ�� insert�� ����(SR = *,OP = i,T = TS[j-1])
				{
					(*SR)[lencount] = '*';
					(*OP)[lencount] = 'i';
					(*TR)[lencount] = TS[j-1];
					j--;
					lencount--;
					continue;
				}
				if (ins_cost > del_cost)   //ins_cost>del_cost�� ��� delete �� ����(SR= SS[i-1],OP = d,T= * )
				{
					(*SR)[lencount] = SS[i-1];
					(*OP)[lencount] = 'd';
					(*TR)[lencount] = '*';
					i--;
					lencount--;
					continue;
				}
			}
			else //parent�� ������ insert���� ���(SR = *,OP = i,T = TS[j-1])
			{
				(*SR)[lencount] = '*';
				(*OP)[lencount] = 'i';
				(*TR)[lencount] = TS[j-1];
				j--;
				lencount--;
				continue;
			}
		}

		if (Table[i - 1][j] + del_cost == Table[i][j])  //parent�� ������ delete�� ���(SR= SS[i-1],OP = d,T= * )
		{
			(*SR)[lencount] = SS[i-1];
			(*OP)[lencount] = 'd';
			(*TR)[lencount] = '*';
			i--;
			lencount--;
			continue;
		}
		
		if (Table[i - 1][j - 1] == Table[i][j])		//S�� i �ε����� T�� j�ε����� ������� subcost�� 0���εд�.
			sbcost = 0;
		else sbcost = sub_cost;  //�ٸ���� �־��� sbcost = sub_cost

		if (sbcost == 0)  //sbcost == 0 �� ���(OP='.',SR=SS[i-1],TR=TS[j-1])
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
		else    //sbcost == sub_cost �� ���.
		{
			if (Table[i - 1][j - 1] + sbcost == Table[i][j])  //parent�� ������ insert,delete �Ѵ� �ƴϰ� substitute �� ���(OP='s',SR=SS[i-1],TR=TS[j-1])
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

	/* SR,OP,TR�� string���� �νĵǷ��� �������� NULL POINTER�� ���� ����� ��*/
	(*SR)[count] = '\0';
	(*OP)[count] = '\0';
	(*TR)[count] = '\0';
	
	*Mem_Allocated =(int)(strlen(*SR)+ strlen(*TR) + strlen(*OP)) + 3;

}