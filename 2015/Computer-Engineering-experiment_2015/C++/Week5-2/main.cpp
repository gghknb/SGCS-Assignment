#include "str.h"

int main()
{
	Str a("I`m a girl");  //a배열을 선언하고 I`m a girl을대입
	cout << a.contents(); // 출력
	a="I`m a boy\n";  // a 배열을 선언하고 I`m a boy 를 대입
	cout << a.contents();//출력
	cout << a.compare("I`m a a") << endl; //a의 내용과 I`m a a내용을 비교
	return 0;
}
