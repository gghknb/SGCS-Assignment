#include "Array.h"
#include <iostream>
using namespace std;


	Array::Array(int size) {  //������
	if(size<=0){
		cout<<"ERROR : size�� 0���� �Դϴ�"<<endl;
		exit(-1);
	}//size�� 0���� ���̸� �����޽��� ����ϰ� ����
	else {
			data= new int[size];
			len=size;// size�� ����̸� new�� ����Ͽ� �迭 data�� �Ҵ�,len�� �ʱ�ȭ
		}
	}
	Array::~Array(){ 
		delete(data); 
		len=0;
	} //�Ҹ���,�Ҵ�� �޸�����
	int Array::length() const{
		return len;
	}//�迭�� ũ�� ����
	//�迭�� ���Ҹ� �����ϰų� ���� ��ȯ�ϴ� �κ����� []�������� ���������̴�.
	int &Array::operator [](int i) { //left value : �迭�� ���Ҹ� ����
		static int tmp;
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error! "<<endl;
				return tmp;
		}
		//�迭�� �ε����� ���� ���� ������ �ش� ���Ҹ� ����
		//�׷��� ������ ���� �޽��� ����ϰ� tmp�� ����
	}
	int Array::operator [](int i)const{ 
		if(i>=0&&i<=len)
			return data[i];
		else {
			cout<<"Array bound error !"<<endl;
			return 0;
		}//right value : �迭�� ������ ���� ��ȯ
		//�迭�� �ε����� ���� ���� ������ ���� ����
		//�׷��� ������ ���� �޼����� ����ϰ� 0�� ����
	}
	void Array::print() {
		cout<<"[ ";
		for(int i=0;i<len;i++)
		{
			cout<<data[i]<<" ";
		}
		cout<<"]"<<endl;
	}//�迭�� ��� ������ ������ִ��Լ�
