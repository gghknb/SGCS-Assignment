#include <iostream>
#include <string.h>
using namespace std;

class Str
{
	private:
		char *str; // string 의 내용
		int len; //string 의 길이
	public:
		Str(int leng); // leng 은 string 의 길이
		Str(char *neyong); // neyong 초기화할 내용이 들어감
		~Str();//소멸자
		int length(void); // string 의 길이를 리턴하는 함수
		char* contents(void); //string의 내용을 리턴하는 함수
		int compare(class Str a); // a의 내용과 strcmp
		int compare(char *a); //a의 내용과 strcmp
		void operator=(char *a); // string 의 값을대입
		void operator=(class Str a); // str내용을 대입
};

