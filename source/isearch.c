#include <isearch.h>
struct ielem* add_elem(struct ielem* root, char* name, enum types type) //���������� ����� ����������
{
	if (strlen(root->name) == 0)return init(root, name);
	struct ielem* current = (struct ielem*)malloc(sizeof(struct ielem));
	struct ielem* t = (struct ielem*)malloc(sizeof(struct ielem));
	t = root;
	strcpy(current->name, name);
	current->type = type;
	current->next = NULL;
	while (root->next != NULL) root = root->next; //������� �� ����������
	current->prev = root;
	root->next = current;
	return t;
}

struct ielem* init(struct ielem* root, char* name, enum types type) //������������� ������ ����������
{
	root->type = type;
	strcpy(root->name, name);
	root->next = NULL;
	root->prev = NULL;
	return root;
}

struct ielem* del_elem(struct ielem* root, char* name) //�������� ��������, ������������ �������� - ��� ������ ��� ���
{
	if (strlen(root->name) == 0) return root;
	if (!isearch(root, name)) return root;
	while (strcmp(root->name, name)) root = root->next; //����� �� ���� ��� ����� ��������
	struct ielem* t = (struct ielem*)malloc(sizeof(struct ielem));
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

bool isearch(struct ielem* root, char* name)
{
	while (strcmp(root->name, name) && root->next!=NULL) root = root->next;
	if (!strcmp(root->name, name)) return 1;
	else return 0;
}

void printo(struct ielem* root) //��� �������
{
	printf("%s", root->name);
	while (root->next != NULL)
	{
		root = root->next;
		printf(", %s", root->name);
	}
	printf(".\n");
}