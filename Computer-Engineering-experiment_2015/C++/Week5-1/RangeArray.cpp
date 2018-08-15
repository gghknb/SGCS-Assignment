#include "RangeArray.h"
#include <iostream>
using namespace std;

RangeArray::RangeArray(int a, int b) : Array(b-a+1) //»ó¼Ó
{
	low=a;
	high=b;
}
RangeArray::~RangeArray()
{}

int RangeArray::baseValue()
{
	return low;
}
int RangeArray::endValue()
{
	return high;
}

int& RangeArray::operator[](int i)
{
	static int tmp;
	if(i>=low&&i<=high)
	{
		return data[i-low];
	}
	else{
		cout<<"Array bound error!"<<endl;
		return tmp;
	}

}
int RangeArray::operator[](int i) const
{
	if(i>=low&&i<=high)
	{
		return data[i-low];
	}
	else {
		cout<<"Array bound error!"<<endl;
		return 0;
	}
}

