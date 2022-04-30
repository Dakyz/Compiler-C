#include "isearch.h"
struct idElem* add_elem(struct idElem* root, char* name, enum types type) //добавление новой переменной
{
	if (strlen(root->name) == 0)return init(root, name);
	struct idElem* current = (struct idElem*)malloc(sizeof(struct idElem));
	struct idElem* t = (struct idElem*)malloc(sizeof(struct idElem));
	t = root;
	strcpy(current->name, name);
	current->type = type;
	current->next = NULL;
	while (root->next != NULL) root = root->next; //доходим до последнего
	current->prev = root;
	root->next = current;
	return t;
}

struct idElem* init(struct idElem* root, char* name, enum types type) //инициализация первой переменной
{
	root->type = type;
	strcpy(root->name, name);
	root->next = NULL;
	root->prev = NULL;
	return root;
}

struct idElem* del_elem(struct idElem* root, char* name) //удаление элемента, возвращаемое значение - был удален или нет
{
	if (strlen(root->name) == 0) return root;
	if (!isearch(root, name)) return root;
	while (strcmp(root->name, name)) root = root->next; //дошли до того что нужно выпилить
	struct idElem* t = (struct idElem*)malloc(sizeof(struct idElem));
	t = root; //запоминаем что над удалить
	if (root->prev != NULL)
	{
		root = root->prev;
		t = t->next;
		if (root != NULL) root->next = t;
	} //удаляем указатель у предыдущей
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
	if (t != NULL) t->prev = root; //удаляем указатель у следующей если существует
	while (root->prev != NULL) root = root->prev; //в начало
	return root;
}

bool isearch(struct idElem* root, char* name)
{
	while (strcmp(root->name, name) && root->next!=NULL) root = root->next;
	if (!strcmp(root->name, name)) return 1;
	else return 0;
}

void printo(struct idElem* root) //для отладки
{
	printf("%s", root->name);
	while (root->next != NULL)
	{
		root = root->next;
		printf(", %s", root->name);
	}
	printf(".\n");
}