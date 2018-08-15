#include "Graph_adj_list.h"
#include "Graph_adj_list_extern.h"
#include "Graph_adj_list_function.h"

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// Read graph information and form an adjacent list
	int i;
	/* initialize Vertex*/
	for (i = 0; i < Vnum; i++)
	{
		V[i].p = NULL;
		V[i].name = i;
	}
	/* read and form list*/
	for (i = 0; i < Enum; i++)
	{
		scanf_s("%d %d %d",&E[i].vf,&E[i].vr,&E[i].cost);
		E[i].name = i;
		pushQ_ptr_L(&V[E[i].vf].p,E[i].name);
		pushQ_ptr_L(&V[E[i].vr].p,E[i].name);
	}

	
}

void Free_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// remove the links (ptr_Ls) for the adjacent list
	int i;
	for (i = 0; i < Enum; i++)
	{
		popQ_ptr_L(&V[E[i].vf].p);
		popQ_ptr_L(&V[E[i].vr].p);
	}
}

void DFS_Tree_adj_list (
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E,     // edge structure array (starting index 0)
	int    *cost   // accumulated tree cost
) {
	// preform DFS and set the flags of edges in the DFS tree
	// return the DFS tree cost.
	// Recursively do DFS.
	ptr_L *node;
	node = V[src].p;

	V[src].flag = true;
	while (node)
	{
		if (V[E[node->i].vf].flag == false)
		{
			E[node->i].flag = true;
			*cost += E[node->i].cost;
			DFS_Tree_adj_list(E[node->i].vf, Vnum, V, Enum, E, cost);
		}
		else if (V[E[node->i].vr].flag == false)
		{
			E[node->i].flag = true;
			*cost += E[node->i].cost;
			DFS_Tree_adj_list(E[node->i].vr, Vnum, V, Enum, E, cost);
		}
		node = node->p;
	}
}