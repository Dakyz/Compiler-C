#pragma once
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <list>

/*bool isearch(struct idElem* root, char* name); //���������� �� ������ �������������
struct idElem* init(struct idElem* root, char* name, enum types type); //������������� ������ (������ ����������)
struct idElem* add_elem(struct idElem* root, char* name, enum types type); //���������� �������� � ������
struct idElem* del_elem(struct idElem* root, char* name); //�������� ��������
void printo(struct idElem* root); //����� ���� ���������� ����� ������� �� �����*/
//void deinit(); //������������ ������ �� ����� ���� ��� ���

struct idElem
{
	char name[256]; //�������������
	int type; //������������� ����
	bool initialized;
};

/*enum types //���������� ����
{
	"int" = 0, "signed" = 0, "signed int" = 0,
	"char" = 1, 
	"signed char" = 2, 
	"unsigned char" = 3,
	"short" = 4, "short int" = 4, "signed short" = 4, "signed short int" = 4,
	"usigned short" = 5, "unsigned short int" = 5,
	"long" = 6, "long int" = 6, "signed long" = 6, "signed long int" = 6,
	"usigned" = 7, "unsigned int" = 7,
	"unsigned long" = 8, "unsigned long int" = 8,
	"long long" = 9, "long long int" = 9, "signed long long" = 9, "signed long long int" = 9,
	"unsigned long long" = 10, "unsigned long long int" = 10,
	"float" = 11,
	"double" = 12,
	"long double" = 13
	//struct enum union ����� ����� �� �� ��� � ����������� ��������
};*/

enum types {
	void_,
	int_,
	char_,
	float_,
	struct_,
	union_,
	enum_
};

template<class T>
class List 
{
	std::list<T*> members;

public:
	T* localTmp;

	List();
	~List();

	bool isFind(char* id);
	void clear();
	void push_back();
	void print();
};

template<class T>
List<T>::List() : members() {
	this->localTmp = (T*)calloc(sizeof(T), sizeof(T));
}

template<class T>
List<T>::~List() {
	free(this->localTmp);
}
template<class T>
inline bool List<T>::isFind(char* id)
{
	for (auto element = members->begin();
		element != members->end(); ++element) {
		if (!strcmp((*element)->name, id)) {
			return true;
		}
	}
	return false;
}

template<class T>
void List<T>::clear() {
	memset(localTmp, '\0', sizeof(T));
}
template<class T>
void List<T>::push_back() {
	members.push_back(localTmp);
}

template<class T>
void List<T>::print() {

	for (auto element = members.begin();
		element != members.end(); ++element) {
		std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << std::endl;
	}
}