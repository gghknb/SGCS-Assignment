#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int hours;
	int minutes;
	int seconds;
}Time;

int main(void)
{
	Time *prevTimePtr;
	Time currTime;	
	time_t timer;
	struct tm *t;
	char *str;
	int p=1;
	str=(char *)malloc(sizeof(char)*100);
	while(1)
	{	
		timer=time(NULL);
		t=localtime(&timer);
		prevTimePtr->hours=t->tm_hour;
		prevTimePtr->minutes=t->tm_min;
		prevTimePtr->seconds=t->tm_sec;
		printf("system> ");
		gets(str);	
		timer=time(NULL);
		t=localtime(&timer);
		if(strcmp(str,"check")==0)
		{
			currTime.hours=t->tm_hour;
			currTime.minutes=t->tm_min;
			currTime.seconds=t->tm_sec;
			printf("\n\tCheck Point #%02d\n",p);
			printf("\tCurrent Time = %02d : %02d : %02d\n",currTime.hours,currTime.minutes,currTime.seconds);
			printf("\tPrevious Time = %02d : %02d : %02d\n",prevTimePtr->hours,prevTimePtr->minutes,prevTimePtr->seconds);
			currTime.hours=currTime.hours-prevTimePtr->hours;
			currTime.minutes=currTime.minutes-prevTimePtr->minutes;
			currTime.seconds=currTime.seconds-prevTimePtr->seconds;	
			p++;
			if(currTime.seconds<0)
			{
				currTime.seconds=60+currTime.seconds;
				currTime.minutes--;
			}
			else if(currTime.minutes<0)
			{
				currTime.minutes=60+currTime.minutes;
				currTime.hours--;
			}
			else if(currTime.hours<0)
			{
				currTime.hours=24+currTime.hours;
			}
			printf("\tElapsed Time = %02d : %02d : %02d\n\n",currTime.hours,currTime.minutes,currTime.seconds);
		}
		else if(strcmp(str,"quit")==0)
			break;
		else continue;
	}
}

