#include "linkstack.h"

int main()
{
	double dVal;
	string strVal;
	LinkedList<double> dList;
	LinkedList<string> strList;
	dList.Insert(3.14);
	dList.Insert(123456);
	dList.Insert(-0.987654);
	dList.print();
	dList.Delete(dVal);
	cout<<"삭제된 마지막 원소: "<<dVal<<endl;
	dList.print();
	dList.Insert(777.777);
	dList.print();
	dList.Delete(dVal);
	cout<<"삭제된 마지막 원소: "<<dVal<<endl;
	dList.Delete(dVal);
	cout<<"삭제된 마지막 원소: "<<dVal<<endl;
	dList.print();
	dList.Delete(dVal);
	cout<<"삭제된 마지막 원소: "<<dVal<<endl;
	dList.print();
	
	strList.Insert("This");
	strList.Insert("is a");
	strList.Insert("Template");
	strList.Insert("Example");
	strList.print();
	strList.Delete(strVal);
	cout<<"삭제된 마지막 원소: "<<strVal<<endl;
	strList.Insert("Class");
	strList.print();

	return 0;
}