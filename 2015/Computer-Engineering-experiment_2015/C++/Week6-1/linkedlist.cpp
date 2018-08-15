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
	cout<<"������ ������ ����: "<<dVal<<endl;
	dList.print();
	dList.Insert(777.777);
	dList.print();
	dList.Delete(dVal);
	cout<<"������ ������ ����: "<<dVal<<endl;
	dList.Delete(dVal);
	cout<<"������ ������ ����: "<<dVal<<endl;
	dList.print();
	dList.Delete(dVal);
	cout<<"������ ������ ����: "<<dVal<<endl;
	dList.print();
	
	strList.Insert("This");
	strList.Insert("is a");
	strList.Insert("Template");
	strList.Insert("Example");
	strList.print();
	strList.Delete(strVal);
	cout<<"������ ������ ����: "<<strVal<<endl;
	strList.Insert("Class");
	strList.print();

	return 0;
}