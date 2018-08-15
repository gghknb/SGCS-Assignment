#include "Graph_include.h"
#include  <limits>

#define swap(a,b) ((a)^=(b)^=(a)^=(b))

// any function definitions here
void insertheap(int* P1, int* P3, int* P4, int* heap_size);
int deleteheap(int* P1, int* P3, int* P4, int* heap_size);
void adjustheap(int* P1, int* P3, int* P4, int* heap_size,int child);
void findMaxd(int* P1, int* Maxd1, int* Maxd3,int Vnum);
// You must not declare any global variable
int SPT_Dijkstra(		// return SPT cost
	int src,			// source vertex index
	int Vnum, vertex *V, int Enum, edge *E,		// Graph data(the same as in HW04, 05)
	int *P1, int *P2, int *P3, int *P4,			// scratch int  storage of size (Vnum+1)
	bool *P5,									// scratch bool storage of size (Vnum+1)
	int *Maxd1,		// The largest distance (output)
	int *Maxd3      // The third largest distance (output)
	) {
	int i; 
	int heap_size = 0;
	int vfhd, vrhd;
	int u,temp,cost=0;

	/* P1=distance, P2=vertex that pass recently, P3=heap, P4=heap index, P5=visited flag */
	/* initialize P array */
	for (i = 0; i < Vnum; i++)
	{
		P1[i] = INT_MAX;
		P2[i] = i;
		P3[i + 1] = i;
		P4[i] = i + 1;
		P5[i] = false;
	}

	/*initialize a dist that adjacent to src(P1) and vertex that passed recently(P2)*/
	for (vfhd = V[src].f_hd; vfhd != NONE; vfhd = E[vfhd].fp)
	{
		P1[E[vfhd].vr] = E[vfhd].cost;
		P2[E[vfhd].vr] = src;					
	}
	for (vrhd = V[src].r_hd; vrhd != NONE; vrhd = E[vrhd].rp)
	{
		P1[E[vrhd].vf] = E[vrhd].cost;
		P2[E[vrhd].vf] = src;
	}


	P5[src] = true;		
	P1[src] = 0;		//src to src distance =0

	/* Construct min heap */
	for (i = 0; i < Vnum; i++)
		insertheap(P1, P3, P4, &heap_size);
	deleteheap(P1, P3, P4, &heap_size);			 //except src in min heap

	while (heap_size != 0)
	{		
		/*choose a vertex u that s.t P5[u]=false & P1[u] is min */
		u = deleteheap(P1, P3, P4, &heap_size);
		P5[u] = true;
		
		/* Make a spt tree by using DFS*/
		for (vfhd = V[u].f_hd; vfhd != NONE; vfhd = E[vfhd].fp)
		{
			if (P2[u] == E[vfhd].vr)
			{
				E[vfhd].flag = true;			//if edge passed already,then make a edge flag true
				cost = cost + E[vfhd].cost;		//add the cost that past edge .
				break;
			}
		}
		for (vrhd = V[u].r_hd; vrhd != NONE; vrhd = E[vrhd].rp)
		{
			if (P2[u] == E[vrhd].vf)
			{
				E[vrhd].flag = true;			//if edge passed already,then make a edge flag true
				cost = cost + E[vrhd].cost;		//add the cost that past edge .
				break;
			}
		}
		
		/*Get P1[u] by using Dijkstra algorithm  */
		for (vfhd = V[u].f_hd; vfhd != NONE; vfhd = E[vfhd].fp)
		{
			if (P5[E[vfhd].vr] == false && P1[E[vfhd].vr] >= P1[E[vfhd].vf] + E[vfhd].cost)
			{
				P1[E[vfhd].vr] = P1[E[vfhd].vf] + E[vfhd].cost;
				P2[E[vfhd].vr] = u;						//change a P2 if we passed u vertex.
				temp = P4[E[vfhd].vr];
				adjustheap(P1,P3,P4,&heap_size,temp);	//rearrange heap if distance changed
			}
		}
		for (vrhd = V[u].r_hd; vrhd != NONE; vrhd = E[vrhd].rp)
		{
			if (P5[E[vrhd].vf] == false && P1[E[vrhd].vf] >= P1[E[vrhd].vr] + E[vrhd].cost)
			{
				P1[E[vrhd].vf] = P1[E[vrhd].vr] + E[vrhd].cost;
				P2[E[vrhd].vf] = u;						//change a P2 if we passed u vertex.
				temp = P4[E[vrhd].vf];
				adjustheap(P1,P3,P4,&heap_size,temp);	//rearrange heap if distance changed
			}
		}
	}

    findMaxd(P1, Maxd1, Maxd3,Vnum);	//find a Max1 & Max3
	return cost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) { 
	// construct the adjacency list of a graph using array style linked list
	// read graph information
	int i, ef, er;

	/* initialize Vertex*/
	for (i = 0; i < Vnum; i++)
	{
		V[i].f_hd = NONE;
		V[i].r_hd = NONE;
	}
	/* initialize Edge*/
	for (i = 0; i < Enum; i++)
	{
		E[i].vf = NONE;
		E[i].vr = NONE;
		E[i].fp = NONE;
		E[i].rp = NONE;
		E[i].flag = false;
	}

	for (i = 0; i < Enum; i++)
	{
		scanf("%d %d %d", &E[i].vf, &E[i].vr, &E[i].cost);
		ef = E[i].vf;
		er = E[i].vr;

		if (V[ef].f_hd != NONE)
			E[i].fp = V[ef].f_hd;
		V[ef].f_hd = i;
		if (V[er].r_hd != NONE)
			E[i].rp = V[er].r_hd;
		V[er].r_hd = i;
	}
}

// You may write any functions here (eg. heap insert, delete, adjustment)	

void insertheap(int* P1, int* P3, int* P4, int* heap_size)
{
	/* construct a min heap  */
	int child,parent;

	child = *heap_size + 1;
	parent = child / 2;
	P4[P3[child]] = child;
	*heap_size += 1;

	/* reaarange a min heap*/
	while (1)
	{
		if (parent == 0) break;
		if (P1[P3[parent]] > P1[P3[child]])
		{
			swap(P3[child], P3[parent]);
			swap(P4[P3[child]],P4[P3[parent]]);
			child = parent;
			parent /= 2;
		}
		else break;
	}
	return ;
}

int deleteheap(int* P1, int* P3, int* P4, int* heap_size)
{
	/* return a first index of heap and rearrange a heap*/
	int index;
	int parent=1, child;

	index = P3[1];
	P3[1] = P3[*heap_size];		 //change a last index to first index
	P4[P3[*heap_size]] = 1;
	*heap_size -= 1;			

	/* rearrange a min heap */
	while (parent*2 <= *heap_size)
	{
		child = parent * 2;
		if (child == *heap_size)
		{
			if (P1[P3[child]] < P1[P3[parent]])
			{
				swap(P3[child], P3[parent]);
				swap(P4[P3[child]], P4[P3[parent]]);
				parent = child;
			}
			break;
		}
		else if (child != *heap_size && P1[P3[child]] <= P1[P3[child + 1]])
		{
			if (P1[P3[child]] < P1[P3[parent]])
			{
				swap(P3[child], P3[parent]);
				swap(P4[P3[child]],P4[P3[parent]]);
				parent = child;
			}
			else break;
		}
		else if(child != *heap_size && P1[P3[child]] > P1[P3[child + 1]])
		{
			if (P1[P3[child + 1]] < P1[P3[parent]])
			{
				swap(P3[child+1], P3[parent]);
				swap(P4[P3[child+1]],P4[P3[parent]]);
				parent = child + 1;
			}
			else break;
		}
	}
	return index;
}

void adjustheap(int* P1, int* P3, int* P4, int* heap_size, int child)
{
	/* rearrange min heap */
	/* child is a heap index that distance is changed*/
	int parent;

	while (1) {
		parent = child / 2;
		if (parent == 0)break;
		if (P1[P3[parent]]>P1[P3[child]])
		{
			swap(P4[P3[parent]], P4[P3[child]]);
			swap(P3[parent], P3[child]);
			child = parent;
		}
		else break;
	}

	return;
}

void findMaxd(int* P1, int* Maxd1, int* Maxd3, int Vnum)
{
	/*find a Max distance and third Max distance*/
	
	int i, Maxd2;

	/*initailize Maxd1,Maxd2,Maxd3 */
	*Maxd1 = 0;
	Maxd2 = 0;
	*Maxd3 = 0;

	/* get a value Maxd1,Maxd2,Maxd3*/
	for (i = 0; i < Vnum; i++)
	{
		if (*Maxd3 < P1[i])
		{
			if (Maxd2 < P1[i])
			{
				if (*Maxd1 < P1[i])
				{
					*Maxd3 = Maxd2;
					Maxd2 = *Maxd1;
					*Maxd1 = P1[i];
					continue;
				}
				*Maxd3 = Maxd2;
				Maxd2 = P1[i];
				continue;
			}
			*Maxd3 = P1[i];
		}
	}

	return;
}
				



