#include "Array.h"
#include <iostream>
using namespace std;


	Array::Array(int size) {  //생성자
	if(size<=0){
		cout<<"ERROR : size가 0이하 입니다"<<endl;
		exit(-1);
	}//size가 0이하 값이면 에러메시지 출력하고 종료
	else {
			data= new int[size];
			len=size;// size가 양수이면 new를 사용하여 배열 data를 할당,len값 초기화
		}
	}
	Array::~Array(){ 
		delete(data); 
		len=0;
	} //소멸자,할당된 메모리해제
	int Array::length() const{
		return len;
	}//배열의 크기 리턴
	//배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 다중정의이다.
	int &Array::operator [](int i) { //left value : 배열에 원소를 삽입
		static int tmp;
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error! "<<endl;
				return tmp;
		}
		//배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴
		//그렇지 않으면 에러 메시지 출력하고 tmp를 리턴
	}
	int Array::operator [](int i)const{ 
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error !"<<endl;
			return 0;
		}//right value : 배열의 원소의 값을 반환
		//배열의 인덱스가 범위 내에 있으면 값을 리턴
		//그렇지 않으면 에러 메세지를 출력하고 0를 리턴
	}
	void Array::print() {
		cout<<"[ ";
		for(int i=0;i<len;i++)
		{
			cout<<data[i]<<" ";
		}
		cout<<"]"<<endl;
	}//배열의 모든 내용을 출력해주는함수
