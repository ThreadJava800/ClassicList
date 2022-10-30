#include <stdio.h>

#include "list.h"


size_t MAX_LIST_LENGTH = 256;

int main() {
    List_t list = {};
    int err = 0;
    listCtor(&list, 1, &err);
    DUMP(&list, 0);

    ListElement_t *next = _listInsertPhys(&list, 5, list.zero);
    DUMP(&list, 0);

    _listInsertPhys(&list, 5, next);
    DUMP(&list, 0);

    listDtor(&list, &err);

    return 0;
}