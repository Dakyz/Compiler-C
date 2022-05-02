#pragma once
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <list>

struct idElem
{
	char name[256]; //идентификатор
	int type; //идентификатор типа
	bool initialized;
};

enum types {
	void_ = 0,
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
	members.clear();
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

	T* ptr = (T*)calloc(sizeof(T), sizeof(T));
	strncpy(ptr->name, localTmp->name, 256);
	ptr->type = localTmp->type;
	members.push_back(ptr);
}

template<class T>
void List<T>::print() {

	for (auto element = members.begin();
		element != members.end(); ++element) {
		std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << std::endl;
	}
}