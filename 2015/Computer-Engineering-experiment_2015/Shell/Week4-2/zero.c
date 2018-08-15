#include "myheader.h"

int zero_zero(char *num,int expon,int now) {
	int result = 0,i;
	for ( i = 0 ; i < strlen(num) ; i++ ) {
		if ( i < now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
		else if ( i == now )
			result -= expon*10;
		else if ( i > now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
	}
	return result+1;
}
