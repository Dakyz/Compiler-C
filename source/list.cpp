#include "isearch.h"

template<class T>
inline bool List<T>::isFind(char* id)
{
	for (std::list<struct idElem>::iterator element = members->begin();
		element != members->end(); ++element) {
		if (!strcmp(element->name, id)) {
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
