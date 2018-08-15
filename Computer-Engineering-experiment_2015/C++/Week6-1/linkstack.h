#include <string>
#include <iostream>
using namespace std;
//Linked List Node
template <class T>
class Node{
public:
		T data;
		Node *link;
		Node(T element)
		{
			data=element;
			link=NULL;
		}
};
//Linkled List class

template <class T>
class LinkedList
{
protected:
		Node<T> *first;
		int current_size;
public:
		LinkedList()
		{
			first=0;
			current_size=0;
		};
		int GetSize(){
			return current_size;
		}
		void Insert(T element); //맨앞에 원소를 삽입
		virtual bool Delete(T &element); //맨 뒤의 원소를 삭제
		void print(); //리스트를 출력
};


template <class T>
void LinkedList<T>::Insert(T element)
{
	Node<T> *newnode= new Node<T>(element);
	newnode->link = first;
	first = newnode;
	current_size++;
}

template <class T>
bool LinkedList<T>::Delete(T &element)
{
	if(first==0)
		return false;

	Node<T> *current = first;
	Node<T> *previous = NULL;

	while(1)
	{
		if(current->link ==0)  //find end node
		{
			if(previous) 	previous->link = current->link;
			else first = first->link;
			break;
		}
		previous = current;
		current = current->link;
	}
	element = current -> data;
	delete current;

	current_size--;

	return true;
}

template <class T>
void LinkedList<T>::print()
{
	if(first ==0)
		return ;
	Node<T> *current = first;
	int num =1;

	while(current)
	{
		if(num != 1)
			cout<<"->";
		cout<<"["<<num<<"|"<<current->data<<"]";
		current = current ->link;
		num++;
	}

	cout<<endl;

}

template <class T>
class Stack:public LinkedList<T>
{
		
	void Insert(T element); //맨앞에 원소를 삽입
		virtual bool Delete(T &element); //맨 뒤의 원소를 삭제
};


template <class T>
void Stack<T>::Insert(T element)
{
	Node<T> *newnode= new Node<T>(element);
	newnode->link = first;
	first = newnode;
	current_size++;
}

template <class T>
bool Stack<T>::Delete(T &element)
{
	if(!first)
		return false;

	Node<T> *temp;
	temp=first;
	first=first->link;
	delete temp;
	current_size--;
	return true;
}