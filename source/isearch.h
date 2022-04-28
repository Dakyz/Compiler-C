#include <stdio.h>
#include <string.h>
bool isearch(struct ielem* root, char* name); //���������� �� ������ �������������
struct ielem* init(struct ielem* root, char* name, enum types type); //������������� ������ (������ ����������)
struct ielem* add_elem(struct ielem* root, char* name, enum types type); //���������� �������� � ������
struct ielem* del_elem(struct ielem* root, char* name); //�������� ��������
void printo(struct ielem* root); //����� ���� ���������� ����� ������� �� �����
//void deinit(); //������������ ������ �� ����� ���� ��� ���

struct ielem
{
	char name[256]; //�������������
	enum types type; //������������� ����
	struct ielem* next;
	struct ielem* prev;
};

enum types //���������� ����
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
};
