#include "myheader.h"

int main(void)
{
	int tc;
	scanf("%d",&tc);
	while ( tc-- ) {
		int i,j,numcount[10]={0},expon=1;
		char num[11] = {0};
		scanf("%s",num);
		if ( strlen(num) == 1 )
			for ( i = 1 ; i <= num[0]-'0' ; i++ )
				numcount[i]++;
		else {
			for ( i = 0 ; i < strlen(num)-2 ; i++ )
				expon *= 10;
			for ( i = 0 ; i < strlen(num) ; i++ ) {
				if ( i == 0 )
					for ( j = num[i]-'0' ; j >= 1 ; j-- ) {
						if ( j == num[i]-'0' ) numcount[j] += stay(num,expon,i);
						else numcount[j] += expon*10;
					}
				else {
					for ( j = 9 ; j >= 1 ; j-- ) {
						if ( j > num[i]-'0' ) numcount[j] += up(num,expon,i);
						else if ( j < num[i]-'0' ) numcount[j] += down(num,expon,i);
						else numcount[j] += stay(num,expon,i);
					}
					if ( i == strlen(num)-1 ) numcount[0] += up(num,expon,i);
					else if ( num[i]-'0' == 0 ) numcount[0] += zero_zero(num,expon,i);
					else numcount[0] += up(num,expon,i);
				}
			}
		}
		for ( i = 0 ; i < 10 ; i++ )
			printf("%d ",numcount[i]);
		printf("\n");
	}
	return 0;
}
