#include "myheader.h"

int down(char *num,int expon,int now) {
	int result = 0,i;
	for ( i = 0 ; i < strlen(num) ; i++ ) {
		if ( i < now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
		else if ( i > now ) {
			result += expon*10;
			return result;
		}
	}
	if ( now == strlen(num)-1 ) return result+1;
	else    return result;
}
int up(char *num,int expon,int now) {
	int result = 0,i;
	for ( i = 0 ; i < strlen(num) ; i++ ) {
		if ( i < now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
		else if ( i > now ){
			return result;
		}
	}
	return result;
}
int stay(char *num,int expon,int now) {
	int result = 0,i;
	for ( i = 0 ; i < strlen(num) ; i++ ) {
		if ( i < now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
		else if ( i > now ) {
			result += (num[i]-'0')*expon;
			expon /= 10;
		}
	}
	return result+1;
}
