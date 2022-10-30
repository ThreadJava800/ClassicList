#include "list.h"
#include "colors.h"

#if _DEBUG
FILE *logFile = fopen("logs.txt", "w");
int   onClose = atexit(closeLogfile);
#endif

long grDumpCounter = 0;

void _listCtor(List_t *list, short needLinear, int *err) {
    CHECK(!list, LIST_NULL);

    list->zero = (ListElement_t*) calloc(1, sizeof(ListElement_t));
    CHECK(!list->zero, CANNOT_ALLOC_MEM);
    
    list->zero->value    = POISON;
    list->zero->next     = list->zero;
    list->zero->previous = list->zero;

    list->needLinear = needLinear;
    list->size       = 0;

    if (err) *err = listVerify(list);
}

int listVerify(List_t *list) {
    if (!list)                          return LIST_NULL;
    if (!list->zero)                    return LIST_DATA_NULL;
    if (list->size == POISON)           return LIST_SIZE_POISONED;
    if (list->size < 0)                 return LIST_SIZE_POISONED;

    return LIST_OK;
}

ListElement_t* _listInsertPhys(List_t *list, Elem_t value, ListElement_t *index, int *err) {
    CHECK(!list, LIST_NULL);

    if (index != list->zero->previous) {
        list->linearized = 0;
    }

    ListElement_t *pushInd  = (ListElement_t *) calloc(1, sizeof(ListElement_t));

    pushInd->value     = value;
    pushInd->next      = index->next;
    pushInd->previous  = index;

    index->next->previous = pushInd;
    index->next           = pushInd;
    list->size++;

    if (err) *err = listVerify(list);

    return pushInd;
}

// ListElement_t* listInsert(List_t *list, Elem_t value, long index, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(index > list->capacity, INDEX_BIGGER_SIZE);

//     long physIndex = logicToPhysics(list, index);
//     return _listInsertPhys(list, value, physIndex, err);
// }

// ListElement_t* listPushBack(List_t *list, Elem_t value, int *err) {
//     CHECK(!list, LIST_NULL);

//     long lastInd = list->values[0].previous;
//     return _listInsertPhys(list, value, lastInd, err);
// }

// ListElement_t* listPushFront(List_t *list, Elem_t value, int *err) {
//     CHECK(!list, LIST_NULL);

//     return _listInsertPhys(list, value, 0, err);
// }

// Elem_t _listRemovePhys(List_t *list, ListElement_t *index, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(index > list->capacity || index <= 0, INDEX_BIGGER_SIZE);
//     CHECK(list->size == 0, NOTHING_TO_DELETE);
//     CHECK(list->values[index].value == POISON, ALREADY_POISON);

//     if (index != list->values[0].previous) {
//         list->linearized = 0;
//     }

//     Elem_t returnValue = list->values[index].value;

//     list->values[index].value = POISON;

//     list->values[list->values[index].previous].next     = list->values[index].next;
//     list->values[list->values[index].next]    .previous = list->values[index].previous;
//     list->values[index]                       .next     = list->free;
//     list->values[index]                       .next     = list->free;
//     list->values[index]                       .previous = -1;

//     list->size--;
//     list->free = index;

//     return returnValue;
// }

// Elem_t listRemove(List_t *list, long index, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(index > list->capacity, INDEX_BIGGER_SIZE);
//     CHECK(list->size == 0, NOTHING_TO_DELETE);

//     long physIndex = logicToPhysics(list, index);
//     return _listRemovePhys(list, physIndex, err);
// }

// [[nodiscard]] ListElement_t *logicToPhysics(List_t *list, long logicIndex, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(logicIndex > list->capacity, INDEX_INCORRECT);

//     if (list->size == 0) return 0;

//     long pos = list->values[0].next;
//     if (logicIndex == 0) return pos;

//     if (list->linearized) return logicIndex;

//     for (long i = 0; i < logicIndex; i++) {
//         if (list->values[pos].next == 0) {
//             return pos;
//         }
//         pos =  list->values[pos].next;
//     }

//     return pos;
// }

// void listLinearize(List_t *list, int *err) {
//     CHECK(!list, LIST_NULL);

//     if (list->linearized) return;

//     ListElement_t *elements = (ListElement_t *) calloc((size_t) list->capacity + 1, sizeof(ListElement_t));
//     CHECK(!elements, CANNOT_ALLOC_MEM);

//     fillElemList(elements, list->capacity, err);

//     long oldIndex = list->values[0].next;
//     for (long i = 0; i < list->size; i++) {
//         elements[i + 1].value = list->values[oldIndex].value;

//         if (i + 1 >= list->size) elements[i + 1].next  = 0;
//         else                     elements[i + 1].next  = i + 2;

//         if (i == 0) elements[i + 1].previous = 0;
//         else        elements[i + 1].previous =  i;

//         oldIndex =  list->values[oldIndex].next;
//     }

//     elements[0].next = 1;
//     elements[0].previous = list->size;

//     free(list->values);
//     list->values     = elements;
//     list->linearized = 1;
//     list->free       = list->size + 1;

//     if (err) *err = listVerify(list);
// }

// void listResize(List_t *list, long newCapacity, int *err) {
//     CHECK(!list, LIST_NULL);

//     if (list->needLinear) {
//         listLinearize(list, err);
//     }

//     if (newCapacity < list->capacity) {
//         // checking if no sensible data will be deleted
//         if (checkForPoisons(list, newCapacity, err)) {
//             listRealloc(list, newCapacity, err);
//             list->values[newCapacity - 1].next = 0;
//         } else {
//             if (err) *err = LOSING_DATA;
//         }

//         return;
//     }

//     long oldCapacity = list->capacity;
//     listRealloc(list, newCapacity, err);
//     poisonList (list, newCapacity, oldCapacity, err);
// }

// int checkForPoisons(List_t *list, long newCapacity, int *err) {
//      CHECK(!list, LIST_NULL);

//      long goodCounter = 0;
//      for (long i = list->capacity - 1; i > 0; i--) {
//         if (list->values[i].value == POISON) {
//             goodCounter++;
//         }

//         if (goodCounter >  list->capacity - newCapacity) return 1;
//      }  

//      return 0; 
// }

// void listRealloc(List_t *list, long newCapacity, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(!list->values, LIST_DATA_NULL);

//     list->values = (ListElement_t *) realloc(list->values, (size_t) newCapacity * sizeof(ListElement_t));
//     CHECK(!list->values, CANNOT_ALLOC_MEM);

//     list->capacity = newCapacity;
// }

// void poisonList(List_t *list, long newCapacity, long oldCapacity, int *err) {
//     CHECK(!list, LIST_NULL);
//     CHECK(!list->values, LIST_DATA_NULL);
//     CHECK(newCapacity < oldCapacity, INDEX_INCORRECT);

//     list->values[list->free].next =  oldCapacity;

//     for (long i = oldCapacity; i < newCapacity; i++) {
//         list->values[i].value    = POISON;
//         list->values[i].previous = -1;

//         if (i == newCapacity - 1) {
//             list->values[i].next = 0;
//         } else {
//             list->values[i].next =  i + 1;
//         }
//     }
// }

void listDtor(List_t *list, int *err) {
    CHECK(!list, LIST_NULL);

    ListElement_t *ind = list->zero;
    for (long i = 0; i < list->size; i++) {
        ListElement_t *nextInd = ind->next;
        free(ind);
        ind = nextInd;
    }

    list->size       = POISON;
    list->linearized = 1;
}

// void visualGraph(List_t *list, const char *action) {
//     if (!list) return;

//     FILE *tempFile = fopen("temp.dot", "w");
//     if (!tempFile) return;

//     mprintf(tempFile, "digraph structs {\n");
//     mprintf(tempFile, "\trankdir=LR;\n");

//     mprintf(
//                 tempFile, 
//                 "\tinfo[shape=record, style=\"rounded, filled\", fillcolor=\"#d0d1f2\", label=\"{{free: %ld} | \
//                     {size: %ld | cap: %ld} | {linearized: %d |needLinear: %d} }\"];\n", 
//                 list->free,
//                 list->size,
//                 list->capacity,
//                 list->linearized,
//                 list->needLinear
//             );

//     for (long i = 0; i < list->capacity; i++) {
//         char color[MAX_COMMAND_LENGTH];
//         if (i == 0) {
//             strcpy(color, NULL_BLOCK);
//         } else if (list->values[i].value == POISON) {
//             strcpy(color, FREE_BLOCK);
//         } else {
//             strcpy(color, WORK_BLOCK);
//         }

//         mprintf(
//                     tempFile, 
//                     "\tlabel%ld[shape=record, style=\"rounded, filled\", fillcolor=\"%s\", label=\"{index: %ld | val: %d | {n: %ld | p: %ld} }\"];\n", 
//                     i, 
//                     color,
//                     i,
//                     list->values[i].value, 
//                     list->values[i].next,
//                     list->values[i].previous
//                 );
//     }

//     for (long i = 0; i < list->capacity - 1; i++) {
//         mprintf(tempFile, "\tlabel%ld->label%ld [color=\"%s\", style=\"dashed\",arrowhead=\"none\"]", i, i + 1, PHYS_LINK);
//     }

//     // from 0 to 0
//     mprintf(tempFile, "\tlabel0->label0 [dir=both, color=\"red:blue\"]\n");

//     long index = list->values[0].next;
//     for (long i = 0; i < list->size; i++) {
//         long nextIndex =  list->values[index].next;
//         long prevIndex =  list->values[index].previous;
//         if (nextIndex == 0) break;

//         mprintf(tempFile, "\tlabel%ld->label%ld [color=\"red\"]\n", index, nextIndex);
//         if (prevIndex) mprintf(tempFile, "\tlabel%ld->label%ld [color=\"blue\"]\n", index, prevIndex);
//         index = nextIndex;
//     }

//     mprintf(tempFile, "\tlabel%ld->label%ld [color=\"blue\"]\n", list->values[0].previous, list->values[list->values[0].previous].previous);

//     index = list->free;
//     for (long i = 0; i < list->capacity - list->size; i++) {
//         long nextIndex =  list->values[index].next;
//         if (nextIndex == 0) break;

//         mprintf(tempFile, "\tlabel%ld->label%ld [color=\"#038c61\"]\n", index, nextIndex);
//         index = nextIndex;
//     }

//     mprintf(tempFile, "}");

//     fclose(tempFile);

//     char command[MAX_COMMAND_LENGTH] = {};
//     sprintf(command, "dot -Tsvg temp.dot > img%ld.svg", grDumpCounter);
//     system(command);

//     // adding to html
//     FILE* graphFile = nullptr;
//     if (grDumpCounter == 0) {
//         graphFile = fopen("gdump.html", "w");
//     } else {
//         graphFile = fopen("gdump.html", "a");
//     }
//     if (!graphFile) return;

//     fprintf(graphFile, "<pre>\n");
//     fprintf(graphFile, "<hr>\n<h2>%s </h2>\n", action);
//     fprintf(graphFile, "<img src=\"img%ld.svg\" />\n</hr>\n", grDumpCounter);

//     fclose(graphFile);

//     grDumpCounter++;
// }

#if _DEBUG
void mprintf(FILE *file, const char *fmt...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
}

void dumpList(List_t *list, int errorCode, const char *fileName, const char *function, int line) {
    mprintf(logFile, "Assertion failed with code %d\n", errorCode);
    mprintf(logFile, "in %s at %s(%d)\n", function, fileName, line);
    if (!list) {
        mprintf(logFile, "List was null\n");
        return;
    }

    mprintf(logFile, "List_t[%p] '%s' at %s at %s(%d)\n", list, list->debugInfo.name, list->debugInfo.createFunc, list->debugInfo.createFile, list->debugInfo.createLine);
    mprintf(logFile, "\tsize = %ld\n", list->size);
    mprintf(logFile, "\tlinearized = %d\n", list->linearized);

    if (!list->zero) {
        mprintf(logFile, "Values are null\n");
        return;
    }

    mprintf(logFile, "Values:\n\t");
    ListElement_t *index = list->zero;   
    for (long i = 0; i < list->size + 1; i++) {
        mprintf(logFile, "%9d (%p)", index->value, index);
        index = index->next;
    }

    mprintf(logFile, "\nNext:\n\t");
    index = list->zero;   
    for (long i = 0; i < list->size + 1; i++) {
        mprintf(logFile, "%25p ", index->next);
        index = index->next;
    }

    mprintf(logFile, "\nPrevious:\n\t");
    index = list->zero;   
    for (long i = 0; i < list->size + 1; i++) {
        mprintf(logFile, "%25p ", index->previous);
        index = index->next;
    }
    mprintf(logFile, "\n\n");
}

void closeLogfile(void) {
    fclose(logFile);
}
#endif