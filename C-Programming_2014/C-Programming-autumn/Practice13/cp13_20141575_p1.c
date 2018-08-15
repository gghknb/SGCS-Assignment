#include <stdio.h>

struct CoinBox{
	int n500,n100,n50,n10;
};

int main()
{
	int money;
	struct CoinBox cb;
	printf("Input money : ");
	scanf("%d",&money);
	cb.n500=money/500;
	money-=money/500*500;
	cb.n100=money/100;
	money-=money/100*100;
	cb.n50=money/50;
	money-=money/50*50;
	cb.n10=money/10;
	printf("# of 500 : %d\n",cb.n500);
	printf("# of 100 : %d\n",cb.n100);
	printf("# of 50 : %d\n",cb.n50);
	printf("# of 10 : %d\n",cb.n10);
	return 0;
}

