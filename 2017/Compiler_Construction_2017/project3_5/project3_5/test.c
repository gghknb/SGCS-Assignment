/* test.c for floyd
   read N : the number of nodes
   and find shortest path cost of a to b (a, b in N) */
int N;
int M;
int d[50];

int input(void) {
	int read;
	return read;
}

void output(int data){
	/* print data!! */
}

int index(int x, int y){
	return x + d[0] * y;
}

void main(void) {
	int i;
	i = 0;

	N = input();
	M = input();

	while (i < N) {
		int j;
		j = 0;
		while (j < N) {
			int k;
			k = 0;
			while (k < N) {
				if (d[index(i, j)] > d[index(i, k)] + d[index(k, j)]){
					d[index(i, j)] = d[index(i, k)] + d[index(k, j)];
				}
                else{
                    int temp;
                    temp=d[index(i,j)]+M;
                }
				k = k + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
}
