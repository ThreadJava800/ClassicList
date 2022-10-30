#include <stdio.h>

#include "list.h"


size_t MAX_LIST_LENGTH = 256;

int main() {
    List_t list = {};
    int err = 0;
    listCtor(&list, &err);
    DUMP(&list, 0);

    listPushBack(&list, 5);
    listPushBack(&list, 6);
    listPushBack(&list, 7);
    listPushBack(&list, 8);

    listPushFront(&list, 9);

    printf("%p", logicToPhysics(&list, 2));
    listInsert(&list, 11, 2);

    DUMP(&list, 9);

    listDtor(&list, &err);

    return 0;
}