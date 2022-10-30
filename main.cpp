#include <stdio.h>

#include "list.h"


size_t MAX_LIST_LENGTH = 256;

int main() {
    List_t list = {};
    int err = 0;
    listCtor(&list, &err);
    visualGraph(&list, "Initialize list");

    listPushBack(&list, 5);
    visualGraph(&list, "Push back 5");

    ListElement_t *sixInd = listPushFront(&list, 6);
    visualGraph(&list, "Push front 6");

    listPushFront(&list, 7);
    visualGraph(&list, "Push front 7");

    ListElement_t *eightInd = listPushFront(&list, 8);
    visualGraph(&list, "Push front 8");

    listInsert(&list, 1, 2);
    visualGraph(&list, "Insert 1 on second position (logical)");

    listRemove(&list, 4);
    visualGraph(&list, "Remove 4 (logical postion)");

    listRemove(&list, 1);
    visualGraph(&list, "Remove 1 (logical postion)");

    _listRemovePhys(&list, eightInd);
    visualGraph(&list, "Remove 8 (from phys position)");

    _listInsertPhys(&list, 200, sixInd);
    visualGraph(&list, "Push 200 after 6 (phys position)");

    listDtor(&list, &err);

    return 0;
}