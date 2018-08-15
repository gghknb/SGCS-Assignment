
#include <iostream>
#include <string>
using namespace std;

template <class T>
class array{
protected:
	T *data;
	int len;
public:
	array(int size); 
	~array();
	int length() const;
	T operator [](int i)const; 
	T &operator [](int i);
	void print(); 
};

template <class T>
array<T>::array(int size) {  //생성자
	if(size<=0){
		cout<<"ERROR : size가 0이하 입니다"<<endl;
		exit(-1);
	}//size가 0이하 값이면 에러메시지 출력하고 종료
	else 
	{
			data= new T[size];
			len=size;// size가 양수이면 new를 사용하여 배열 data를 할당,len값 초기화
	}
}

template <class T>
array<T>::~array(){ 
		delete(data); 
		len=0;
	} //소멸자,할당된 메모리해제

template <class T>
int array<T>::length() const{
		return len;
	}//배열의 크기 리턴
	//배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 다중정의이다.

template <class T>	
T &array<T>::operator [](int i) { //left value : 배열에 원소를 삽입
		static T tmp;
		if(i>=0&&i<=len)
			return data[i];
		else
		{
				cout<<"Array bound error! "<<endl;
				return tmp;
		}
		//배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴
		//그렇지 않으면 에러 메시지 출력하고 tmp를 리턴
}

template <class T>
T array<T>::operator [](int i)const{ 
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error !"<<endl;
			return 0;
		}//right value : 배열의 원소의 값을 반환
		//배열의 인덱스가 범위 내에 있으면 값을 리턴
		//그렇지 않으면 에러 메세지를 출력하고 0를 리턴
}

template <class T>
void array<T>::print()
{
		cout<<"[ ";
		for(int i=0;i<len;i++)
		{
			cout<<data[i]<<" ";
		}
		cout<<"]"<<endl;
	//배열의 모든 내용을 출력해주는함수
}

template <class T>  //array class의 public 부분을 상속받아 GrowableArray class 생성(임의의 자료형을 저장가능하게 해야하므로 template이용
class GrowableArray:public array<T>
{
protected :
	int len;
	T *data2;
public :
	GrowableArray(int x);
	~GrowableArray();
	T &operator[](int i);
	T operator[](int i)const;
};

template <class T>
GrowableArray<T>::GrowableArray(int x):array<T>(x) // 생성자부분 
{
	len = x;
}
template <class T>
GrowableArray<T>::~GrowableArray() 
{
}

template <class T>
T &GrowableArray<T>::operator[](int i) /*range 밖의 인덱스를 사용하여 저장할땐 현재 사이즈의 2배로 증가시키고 추가공간은0으로초기화 */

{
	int j;
	if(i > len)
	{
		data2=new T[len];
		for(j=0;j<len;j++)
			data2[j]=this->data[j];
		array<T>::data= new T[2*i];
		for(j=0;j<len;j++)
			this->data[j]=data2[j];
		for(j=len;j<2*i;j++)
			this->data[j]=0;
		array<T>::len = 2 *i;
	}
	return array<T>::operator[](i);
}

template <class T>
T GrowableArray<T>::operator[](int i)const	{
	return array<T>::operator[](i);
}
