
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
array<T>::array(int size) {  //������
	if(size<=0){
		cout<<"ERROR : size�� 0���� �Դϴ�"<<endl;
		exit(-1);
	}//size�� 0���� ���̸� �����޽��� ����ϰ� ����
	else 
	{
			data= new T[size];
			len=size;// size�� ����̸� new�� ����Ͽ� �迭 data�� �Ҵ�,len�� �ʱ�ȭ
	}
}

template <class T>
array<T>::~array(){ 
		delete(data); 
		len=0;
	} //�Ҹ���,�Ҵ�� �޸�����

template <class T>
int array<T>::length() const{
		return len;
	}//�迭�� ũ�� ����
	//�迭�� ���Ҹ� �����ϰų� ���� ��ȯ�ϴ� �κ����� []�������� ���������̴�.

template <class T>	
T &array<T>::operator [](int i) { //left value : �迭�� ���Ҹ� ����
		static T tmp;
		if(i>=0&&i<=len)
			return data[i];
		else
		{
				cout<<"Array bound error! "<<endl;
				return tmp;
		}
		//�迭�� �ε����� ���� ���� ������ �ش� ���Ҹ� ����
		//�׷��� ������ ���� �޽��� ����ϰ� tmp�� ����
}

template <class T>
T array<T>::operator [](int i)const{ 
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error !"<<endl;
			return 0;
		}//right value : �迭�� ������ ���� ��ȯ
		//�迭�� �ε����� ���� ���� ������ ���� ����
		//�׷��� ������ ���� �޼����� ����ϰ� 0�� ����
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
	//�迭�� ��� ������ ������ִ��Լ�
}

template <class T>  //array class�� public �κ��� ��ӹ޾� GrowableArray class ����(������ �ڷ����� ���尡���ϰ� �ؾ��ϹǷ� template�̿�
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
GrowableArray<T>::GrowableArray(int x):array<T>(x) // �����ںκ� 
{
	len = x;
}
template <class T>
GrowableArray<T>::~GrowableArray() 
{
}

template <class T>
T &GrowableArray<T>::operator[](int i) /*range ���� �ε����� ����Ͽ� �����Ҷ� ���� �������� 2��� ������Ű�� �߰�������0�����ʱ�ȭ */

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
