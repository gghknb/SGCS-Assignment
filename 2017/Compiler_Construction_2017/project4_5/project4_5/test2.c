int square(int n)
{
	return n * n;
}
int doublem(int m)
{
	return m * 2;
}
void save(int x[])
{
	int l;
	l = 0;
	while(l < 10)
	{
		if(l <= 5)
		{
			x[l] = square(x[l]);
		}
		else
		{
			x[l] = doublem(x[l]);
		}
		l = l + 1;
	}
}
void main(void)
{
	int i;
	int x[10];
	i = 0;

	while(i < 10)
	{
		x[i] = input();
		i = i + 1;
	}

	save(x);
	i = 0;
	while(i < 10)
	{
		output(x[i]);
		i = i + 1;
	}
	return;
}
