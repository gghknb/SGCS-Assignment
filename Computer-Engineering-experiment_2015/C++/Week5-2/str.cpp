#include "str.h"

Str::Str(int leng) //(생성자)leng은 string의길이고 str 배열을 정의
{
	str=new char[leng];
	len=leng;
}
Str::Str(char *neyong) //(생성자)neyong은 초기화할 내용이 들어가고  str 배열을 정의
{
	len=strlen(neyong);
	str=new char[len];
	strcpy(str,neyong);
}
Str::~Str()  //소멸자
{
	delete(str);
	len=0;
}
int Str::length(void) // 배열의길이를 return
{
	return len;
}

char* Str::contents(void) //str의 내용을 리턴
{
	return str;
}

int Str::compare(class Str a) // a의 내용과 str을 비교
{
	return strcmp(str,a.contents());
}

int Str::compare(char *a) //a의내용과 str을 비교
{
	return strcmp(str,a);
}

void Str::operator=(char *a)  // string의 값을 대입
{
	strcpy(str,a);
	len=strlen(str);
}

void Str::operator=(class Str a) //str의 값을 대입한다.
{
	strcpy(str,a.contents());
	len=strlen(str);
}




