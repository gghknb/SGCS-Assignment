#include "Graph_adj_array.h"
#include "Graph_adj_array_extern.h"
#include "Graph_adj_array_function.h"

void Read_Graph_adj_array ( ) {
	// read graph information
	int i, ef, er;

	/* initialize Vertex*/
	for (i = 0; i < Vnum; i++)
	{
		V[i].name = i;
		V[i].f_hd = -1;
		V[i].r_hd = -1;
	}
	/* initialize Edge*/
	for (i = 0; i < Enum; i++) 
	{
		E[i].name = i;
		E[i].vf = -1;
		E[i].vr = -1;
		E[i].fp = -1;
		E[i].rp = -1;
	}

	for (i = 0; i < Enum; i++) 
	{
		scanf("%d %d %d", &E[i].vf, &E[i].vr, &E[i].cost);
		ef=E[i].vf;
		er=E[i].vr;

		if (V[ef].f_hd != -1)
			E[i].fp = V[ef].f_hd;
		V[ef].f_hd = E[i].name;
		if (V[er].r_hd != -1)
			E[i].rp = V[er].r_hd;
		V[er].r_hd = E[i].name;
	}
}

void DFS_Tree_adj_array(int src) {
	// src = source node index
	int temp;

	V[src].flag = true;
	temp = V[src].f_hd;
	while(1)
	{
		if (temp == -1)
			break;
		if (E[temp].flag == false) 
		{
			if (V[E[temp].vr].flag == false) 
			{
				E[temp].flag = true;
				Tree_cost += E[temp].cost;
				DFS_Tree_adj_array(E[temp].vr);
			}
		}
		temp = E[temp].fp;
	}

	temp = V[src].r_hd;
	while(1)
	{
		if (temp == -1)
			break;
		if (E[temp].flag == false) 
		{
			if (V[E[temp].vf].flag == false) 
			{
				E[temp].flag = true;
				Tree_cost += E[temp].cost;
				DFS_Tree_adj_array(E[temp].vf);
			}
		}
		temp = E[temp].rp;
	}
}
