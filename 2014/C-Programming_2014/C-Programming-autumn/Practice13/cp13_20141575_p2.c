#include <stdio.h>
#include <string.h>


enum Date{sun,mon,tue,wed,thu,fri,sat};

enum Date getDate(int year,int month,int day)
{
	return (year+year/4-year/100+year/400+(13*month+8)/5+day)%7;
}

char *getDayOfweek(enum Date currDate);

struct Calendar{
	int year,month,day;
	enum Date date;
	char *dateString;
};

int main()
{
	struct Calendar cal;
	printf("Input Year : ");
	scanf("%d",&cal.year);
	printf("Input Month : ");
	scanf("%d",&cal.month);
	printf("Input Day : ");
	scanf("%d",&cal.day);
	cal.date=getDate(cal.year,cal.month,cal.day);
	cal.dateString=getDayOfweek(cal.date);
	printf("%d-%d-%d is %s",cal.year,cal.month,cal.day,cal.dateString);
	return 0;
}

char *getDayOfweek(enum Date currDate)
{
	char *string;	
	switch(currDate)
	{
		case mon : string="Monday"; 
				   break;
		case tue : string="Tuesday"; 
				   break;
		case wed : string="Wednesday"; 
				   break;
		case thu : string="Thursday";
				   break;
		case fri : string="Friday";
				   break;
		case sat : string="Saturday";
				   break;
		case sun : string="Sunday";
				   break;
		default : break; 
	}
	return string;
}
