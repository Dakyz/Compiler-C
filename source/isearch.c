#include "isearch.h"
struct idElem* add_elem(struct idElem* root, char* name, enum types type) //���������� ����� ����������
{
	if (strlen(root->name) == 0)return init(root, name);
	struct idElem* current = (struct idElem*)malloc(sizeof(struct idElem));
	struct idElem* t = (struct idElem*)malloc(sizeof(struct idElem));
	t = root;
	strcpy(current->name, name);
	current->type = type;
	current->next = NULL;
	while (root->next != NULL) root = root->next; //������� �� ����������
	current->prev = root;
	root->next = current;
	return t;
}

struct idElem* init(struct idElem* root, char* name, enum types type) //������������� ������ ����������
{
	root->type = type;
	strcpy(root->name, name);
	root->next = NULL;
	root->prev = NULL;
	return root;
}

struct idElem* del_elem(struct idElem* root, char* name) //�������� ��������, ������������ �������� - ��� ������ ��� ���
{
	if (strlen(root->name) == 0) return root;
	if (!isearch(root, name)) return root;
	while (strcmp(root->name, name)) root = root->next; //����� �� ���� ��� ����� ��������
	struct idElem* t = (struct idElem*)malloc(sizeof(struct idElem));
	t = root; //���������� ��� ��� �������
	if (root->prev != NULL)
	{
		root = root->prev;
		t = t->next;
		if (root != NULL) root->next = t;
	} //������� ��������� � ����������
	else
	{
		if (root->next == NULL) root->name[0] = '\0';
		else
		{
			root = root->next;
			root->prev = NULL;
		}
		return root;
	}
	if (t != NULL) t->prev = root; //������� ��������� � ��������� ���� ����������
	while (root->prev != NULL) root = root->prev; //� ������
	return root;
}

bool isearch(struct idElem* root, char* name)
{
	while (strcmp(root->name, name) && root->next!=NULL) root = root->next;
	if (!strcmp(root->name, name)) return 1;
	else return 0;
}

void printo(struct idElem* root) //��� �������
{
	printf("%s", root->name);
	while (root->next != NULL)
	{
		root = root->next;
		printf(", %s", root->name);
	}
	printf(".\n");
}