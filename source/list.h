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
	bool callable;
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
	std::list<T*> global;
	std::list<T*> members;
	std::list<char*> types;

public:
	T* localTmp;
	char * type;

	List();
	~List();

	bool is_find(char* id, bool isGlobal);
	bool is_init(char* id);
	bool is_type(char* id);

	void clear();
	void clear_members();
	void push_back(bool isGlobal);
	void push_type();
	void print();
	void init_last();
};

template<class T>
List<T>::List() : global(), members(), types() {
	this->localTmp = (T*)calloc(sizeof(T), sizeof(T));
	this->type = (char*)calloc(256, sizeof(char));
}

template<class T>
List<T>::~List() {
	members.clear();
	free(this->localTmp);
}

template<class T>
void List<T>::clear_members() {
	
	members.clear();
}

template<class T>
void List<T>::push_type() {
	char* ptr = (char*)calloc(256, sizeof(T));
	strncpy(ptr, type, 256);
	types.push_back(ptr);
}

template<class T>
void List<T>::init_last() {
	
	T* element = global.back();
	global.pop_back();
	element->initialized = true;
	global.push_back(element);
}

template<class T>
inline bool List<T>::is_find(char* id, bool isGlobal)
{
	for (auto element = members.begin();
		element != members.end(); ++element) {
		if (!strcmp((*element)->name, id)) {
			return true;
		}
	}

	for (auto element = global.begin();
		element != global.end(); ++element) {
		if (!strcmp((*element)->name, id)) {
			if (!(*element)->callable || !isGlobal)
				return true;
		}
	}

	return false;
}

template<class T>
inline bool List<T>::is_init(char *id) {

	for (auto element = members.begin();
		element != members.end(); ++element) {
		if (!strcmp((*element)->name, id)) {
			return (*element)->initialized;
		}
	}

	for (auto element = global.begin();
		element != global.end(); ++element) {
		if (!strcmp((*element)->name, id)) {
			return (*element)->initialized;
		}
	}
	return false;
}

template<class T>
inline bool List<T>::is_type(char *id) {

	for (auto element = types.begin();
		element != types.end(); ++element) {
		if (!strcmp(*element, id)) {
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
void List<T>::push_back(bool isGlobal) {

	T* ptr = (T*)calloc(sizeof(T), sizeof(T));
	strncpy(ptr->name, localTmp->name, 256);
	ptr->type = localTmp->type;
	ptr->initialized = localTmp->initialized;
	ptr->callable = localTmp->callable;
	isGlobal ? global.push_back(ptr) :
		members.push_back(ptr);
}

template<class T>
void List<T>::print() {

	for (auto element = members.begin();
		element != members.end(); ++element) {
		std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << ", "
			<< ((*element)->initialized ? "initialized" : "not initialized") << ", "
			<< ((*element)->callable ? "callable" : "not callable")
			<< std::endl;
	}
	std::cout 
		<< '\n' << '\n' << '\n' << '\n' << '\n';
	//std::cout << "global" << std::endl;

	//for (auto element = global.begin();
	//	element != global.end(); ++element) {
	//	std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << ", "
	//		<< ((*element)->initialized ? "initialized" : "not initialized") << ", "
	//		<< ((*element)->callable ? "callable" : "not callable")
	//		<< std::endl;
	//}
}