#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <list>

/*bool isearch(struct idElem* root, char* name); //существует ли данный идентификатор
struct idElem* init(struct idElem* root, char* name, enum types type); //инициализация списка (первая переменная)
struct idElem* add_elem(struct idElem* root, char* name, enum types type); //добавление элемента в список
struct idElem* del_elem(struct idElem* root, char* name); //удаление элемента
void printo(struct idElem* root); //вывод всех переменных потом удалить ее можно*/
//void deinit(); //освобождение памяти не помню надо или нет

struct idElem
{
	char name[256]; //идентификатор
	int type; //идентификатор типа
	bool initialized;
};

/*enum types //допустимые типы
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
	//struct enum union пашол нахер хз че еще с указателями ссылками
};*/

enum types {
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
	std::list<T> members;

public:
	T* localTmp;

	List() : members() {
		this->localTmp = (T*)calloc(sizeof(T), sizeof(T));
	}
	~List() {
		free(this->localTmp);
	}

	bool isFind(char* id);
	void clear();
	void push_back();
};