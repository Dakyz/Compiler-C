#pragma once
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

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
	std::vector<std::vector<T*>> nested;

public:
	T* localTmp;
	char * type;
	long unsigned int level;

	List();
	~List();

	bool is_find(char* id, bool isGlobal);
	bool is_init(char* id);
	bool is_type(char* id);
	bool is_duplicate();

	void clear_members();
	void nested_clear();
	void up_level();
	void push_back(bool isGlobal);
	void push_type();
	void print();
	void init_last();
};

template<class T>
List<T>::List() : global(), members(), types(), nested(), level(0) {
	this->localTmp = (T*)calloc(sizeof(T), sizeof(T));
	this->type = (char*)calloc(256, sizeof(char));
}

template<class T>
List<T>::~List() {
	clear_members();

	for (auto element = global.begin();
		element != global.end(); ++element) {
		free(*element);
	}

	for (auto element = types.begin();
		element != types.end(); ++element) {
		free(*element);
	}

	free(this->localTmp);
	free(this->type);
}

template<class T>
void List<T>::clear_members() {

	for (auto element = members.begin();
		element != members.end(); ++element) {
		free(*element);
	}
	members.clear();
}

template<class T>
void List<T>::nested_clear() {

	--level;
	auto element = nested[level];
	for (auto element_nested = element.begin();
		element_nested != element.end(); ++element_nested) {
		free(*element_nested);
	}
	nested[level].clear();
	nested.pop_back();
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
void List<T>::up_level() {

	++level;
	nested.push_back(std::vector<T*>());
}

template<class T>
inline bool List<T>::is_find(char* id, bool isGlobal)
{
	for (auto element = nested.begin();
		element != nested.end(); ++element) {
		for (auto element_nested = element->begin();
			element_nested != element->end(); ++element_nested) {
			if (!strcmp((*element_nested)->name, id)) {
				return true;
			}
		}
	}

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
	for (auto element = nested.begin();
		element != nested.end(); ++element) {
		for (auto element_nested = element->begin();
			element_nested != element->end(); ++element_nested) {
			if (!strcmp((*element_nested)->name, id)) {
				return (*element_nested)->initialized;
			}
		}
	}

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
void List<T>::push_back(bool isGlobal) {

	T* ptr = (T*)calloc(sizeof(T), sizeof(T));
	strncpy(ptr->name, localTmp->name, 256);
	ptr->type = localTmp->type;
	ptr->initialized = localTmp->initialized;
	ptr->callable = localTmp->callable;

	if (level > 0) {
		nested[level - 1].push_back(ptr);
	}
	else {
		isGlobal ? global.push_back(ptr) :
			members.push_back(ptr);
	}
	memset(localTmp, '\0', sizeof(T));
}

template<class T>
bool List<T>::is_duplicate() {

	for (auto element = global.begin();
		element != global.end(); ++element) {
		for (auto duplicate = std::next(element, 1);
			duplicate != global.end(); ++duplicate) {
			if (!strcmp((*element)->name, (*duplicate)->name) && 
				(*element)->callable && (*duplicate)->callable) {
				if ((*element)->initialized) {
					global.erase(duplicate);
				}
				else if ((*duplicate)->initialized) {
					global.erase(element);
				}
				else {
					global.erase(duplicate);
				}
				return true;
			}
		}
	}
	return false;
}

template<class T>
void List<T>::print() {

	std::cout << "nested" << std::endl;
	for (auto element = nested.begin();
		element != nested.end(); ++element) {
		for (auto element_nested = element->begin();
			element_nested != element->end(); ++element) {
			std::cout << "id: " << (*element_nested)->name << ", id_type: " << (*element_nested)->type << ", "
				<< ((*element_nested)->initialized ? "initialized" : "not initialized") << ", "
				<< ((*element_nested)->callable ? "callable" : "not callable")
				<< std::endl;
		}
	}

	std::cout << "local" << std::endl;
	for (auto element = members.begin();
		element != members.end(); ++element) {
		std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << ", "
			<< ((*element)->initialized ? "initialized" : "not initialized") << ", "
			<< ((*element)->callable ? "callable" : "not callable")
			<< std::endl;
	}
	std::cout << "global" << std::endl;

	for (auto element = global.begin();
		element != global.end(); ++element) {
		std::cout << "id: " << (*element)->name << ", id_type: " << (*element)->type << ", "
			<< ((*element)->initialized ? "initialized" : "not initialized") << ", "
			<< ((*element)->callable ? "callable" : "not callable")
			<< std::endl;
	}
}