#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define LIST_BY_YEAR       0
#define LIST_BY_TYPE       1
#define TYPE_MAX           100
#define SETUP_MAX          100
typedef struct TEngine {
    struct TEngine *m_Next;
    struct TEngine *m_Prev;
    int m_Year;
    char m_Type[TYPE_MAX];
    int m_Setup[SETUP_MAX];
} TENGINE;
typedef struct TArchive {
    struct TArchive *m_Next;
    struct TArchive *m_Prev;
    TENGINE *m_Engines;
} TARCHIVE;
TENGINE *createEngine(const char *type, int year){
    TENGINE *res = (TENGINE *) malloc(sizeof(*res));
    res->m_Next = NULL;
    res->m_Prev = NULL;
    res->m_Year = year;
    strncpy(res->m_Type, type, sizeof(res->m_Type));
    for (int i = 0; i < SETUP_MAX; i++)
        res->m_Setup[i] = 0;
    return res;
}
#endif /* __PROGTEST__ */
/*void printArchive(TARCHIVE * list){
    TARCHIVE * ptr = list;
    while (ptr){
        TARCHIVE * next = ptr->m_Next;
        TENGINE * ePtr = ptr->m_Engines;
        while (ePtr){
            TENGINE * eNext = ePtr->m_Next;
            printf("%d, %s |",ePtr->m_Year,ePtr->m_Type);
            ePtr = eNext;
        }
        printf("\n");
        ptr = next;
    }
}*/
/**
 * Function to insert engine by type.
 * @param list Pointer to list
 * @param engine Pointer to engine.
 * @return Pointer to list.
 */
TENGINE * insertEngineByType(TENGINE * list, TENGINE * engine){
    TENGINE * ptr = list;
    while (ptr){
        TENGINE * next = ptr->m_Next;
        int res = strcmp(ptr->m_Type,engine->m_Type);
        if (res > 0){
            engine->m_Next = ptr;
            engine->m_Prev = ptr->m_Prev;
            if (ptr->m_Prev != NULL)
                ptr->m_Prev->m_Next = engine;
            ptr->m_Prev = engine;
            if (list->m_Prev == engine)
                list = engine;
            return list;
        } else if (next == NULL){
            engine->m_Next = NULL;
            engine->m_Prev = ptr;
            ptr->m_Next = engine;
            return list;
        }
        ptr = next;
    }
    return list;
}
/**
 * Function to insert archive by year.
 * @param list Pointer to list.
 * @param engine Pointer to engine list.
 * @return Pointer to list.
 */
TARCHIVE * listByYear (TARCHIVE * list, TENGINE * engine){
    TARCHIVE * ptr = list;
    while (ptr){ // Move trough the list.
        TARCHIVE * next = ptr->m_Next;
        int a = ptr->m_Engines->m_Year;
        int b = engine->m_Year;
        if (a == b){ // Same year archive already exists. Add to engine list.
            ptr->m_Engines = insertEngineByType(ptr->m_Engines, engine);
            return list;
        } else if (a > b){ // Found archive node with higher year num. Insert before the node.
            TARCHIVE * newArchiveNode = (TARCHIVE *)malloc(sizeof(*newArchiveNode));
            newArchiveNode->m_Engines = engine;
            newArchiveNode->m_Next = ptr;
            newArchiveNode->m_Prev = ptr->m_Prev;
            if (ptr->m_Prev != NULL)
                ptr->m_Prev->m_Next = newArchiveNode;
            ptr->m_Prev = newArchiveNode;
            if (list->m_Prev == newArchiveNode)
                list = newArchiveNode;
            return list;
        } else if (next == NULL){ // Add to end
            TARCHIVE * newArchiveNode = (TARCHIVE *)malloc(sizeof(*newArchiveNode));
            newArchiveNode->m_Engines = engine;
            newArchiveNode->m_Next = NULL;
            newArchiveNode->m_Prev = ptr;
            ptr->m_Next = newArchiveNode;
            return list;
        }
        ptr = next;
    }
    return list;
}
/**
 * Function to insert engine by year.
 * @param list Pointer to list.
 * @param engine Pointer to engine.
 * @return Pointer to list.
 */
TENGINE * insertEngineByYear(TENGINE * list, TENGINE * engine){
    TENGINE * ptr = list;
    while (ptr){
        TENGINE * next = ptr->m_Next;
        int a = ptr->m_Year;
        int b = engine->m_Year;
        if (a > b){
            engine->m_Next = ptr;
            engine->m_Prev = ptr->m_Prev;
            if (ptr->m_Prev != NULL)
                ptr->m_Prev->m_Next = engine;
            ptr->m_Prev = engine;
            if (list->m_Prev == engine)
                list = engine;
            return list;
        } else if (next == NULL){
            engine->m_Next = NULL;
            engine->m_Prev = ptr;
            ptr->m_Next = engine;
            return list;
        }
        ptr = next;
    }
    return list;
}
/**
 * Function to insert archive into list by type.
 * @param list Pointer to list.
 * @param engine Pointer to engine
 * @return Pointer to list.
 */
TARCHIVE * listByType (TARCHIVE * list, TENGINE * engine){
    TARCHIVE * ptr = list;
    while (ptr){
        TARCHIVE * next = ptr->m_Next;
        int res = strcmp(ptr->m_Engines->m_Type, engine->m_Type);
        if (res == 0){
            ptr->m_Engines = insertEngineByYear(ptr->m_Engines,engine);
            return list;
        } else if (res > 0) {
            TARCHIVE * newArchiveNode = (TARCHIVE *)malloc(sizeof(*newArchiveNode));
            newArchiveNode->m_Engines = engine;
            newArchiveNode->m_Next = ptr;
            newArchiveNode->m_Prev = ptr->m_Prev;
            if (ptr->m_Prev != NULL)
                ptr->m_Prev->m_Next = newArchiveNode;
            ptr->m_Prev = newArchiveNode;
            if (list->m_Prev == newArchiveNode)
                list = newArchiveNode;
            return list;
        } else if (next == NULL){
            TARCHIVE * newArchiveNode = (TARCHIVE *)malloc(sizeof(*newArchiveNode));
            newArchiveNode->m_Engines = engine;
            newArchiveNode->m_Next = NULL;
            newArchiveNode->m_Prev = ptr;
            ptr->m_Next = newArchiveNode;
            return list;
        }
        ptr = next;
    }
    return list;
}
/**
 * Adds engine to the archives.
 * @param list Pointer to archives.
 * @param listBy Listing argument.
 * @param engine Pointer to new engine.
 * @return Pointer to list
 */
TARCHIVE * AddEngine(TARCHIVE * list, int listBy, TENGINE * engine){
    if (list == NULL){ // list is empty, add a new node
        TARCHIVE * newArchiveNode = (TARCHIVE *)malloc(sizeof(*newArchiveNode));
        newArchiveNode->m_Next = NULL;
        newArchiveNode->m_Prev = NULL;
        newArchiveNode->m_Engines = engine;
        list = newArchiveNode;
    } else { // list is not empty, add engine based on listing
        if (listBy == LIST_BY_YEAR){
            list = listByYear(list,engine);
        } else if (listBy == LIST_BY_TYPE){
            list = listByType(list,engine);
        }
    }
    return list;
}
/**
 * Deletes engine list.
 * @param list Pointer to head of list of engines.
 */
void DelEngine(TENGINE * list){
    while (list){
        TENGINE * next = list->m_Next;
        free(list);
        list = next;
    }
}
/**
 * Deletes archive list.
 * @param list Pointer to head of list of archive.
 */
void DelArchive(TARCHIVE * list){
    while (list){
        TARCHIVE * next = list->m_Next;
        DelEngine(list->m_Engines);
        free(list);
        list = next;
    }
}
void DelArchiveKeepEngine(TARCHIVE * list){
    while (list){
        TARCHIVE * next = list->m_Next;
        free(list);
        list = next;
    }
}
/**
 * Reorders the list to desired order.
 * @param list Pointer to list.
 * @param listBy Listing order argument.
 * @return Pointer to list.
 */
TARCHIVE * ReorderArchive(TARCHIVE * list, int listBy){
    TARCHIVE * newArchive = NULL;
    TARCHIVE * ptr = list;
    while(ptr){
        TARCHIVE * next = ptr->m_Next;
        TENGINE * eptr = ptr->m_Engines;
        while (eptr){
            TENGINE * enext = eptr->m_Next;
            eptr->m_Next = NULL;
            eptr->m_Prev = NULL;
            if (listBy == LIST_BY_TYPE)
                newArchive = AddEngine(newArchive,LIST_BY_TYPE,eptr);
            else
                newArchive = AddEngine(newArchive,LIST_BY_YEAR,eptr);
            eptr = enext;
        }
        ptr = next;
    }
    DelArchiveKeepEngine(list);
    return newArchive;
}
#ifndef __PROGTEST__
int main(void) {
    TARCHIVE *a;

    a = NULL;
    a = AddEngine(a, LIST_BY_YEAR, createEngine("TDI 1.9", 2010));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next == NULL);
    a = AddEngine(a, LIST_BY_YEAR, createEngine("TDI 1.9", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_YEAR, createEngine("TSI 1.2", 2010));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_YEAR, createEngine("TDI 2.0", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 2.0"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_YEAR, createEngine("MPI 1.4", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "MPI 1.4"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Next->m_Next->m_Type, "TDI 2.0"));
    assert (a->m_Engines->m_Next->m_Next->m_Prev == a->m_Engines->m_Next);
    assert (a->m_Engines->m_Next->m_Next->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    a = ReorderArchive(a, LIST_BY_TYPE);
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "MPI 1.4"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Prev == a->m_Next);
    assert (a->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Next->m_Engines->m_Type, "TDI 2.0"));
    assert (a->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Prev == a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Next->m_Next->m_Engines->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Next == NULL);
    DelArchive(a);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    a = NULL;
    a = AddEngine(a, LIST_BY_TYPE, createEngine("TDI 1.9", 2010));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next == NULL);
    a = AddEngine(a, LIST_BY_TYPE, createEngine("TDI 1.9", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next == NULL);
    a = AddEngine(a, LIST_BY_TYPE, createEngine("TSI 1.2", 2010));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_TYPE, createEngine("TDI 2.0", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 2.0"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Prev == a->m_Next);
    assert (a->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Next->m_Engines->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_TYPE, createEngine("MPI 1.4", 2005));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "MPI 1.4"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Prev == a->m_Next);
    assert (a->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Next->m_Engines->m_Type, "TDI 2.0"));
    assert (a->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Prev == a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Next->m_Next->m_Engines->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Next == NULL);
    a = AddEngine(a, LIST_BY_TYPE, createEngine("TDI 1.9", 2010));
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "MPI 1.4"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Prev == a->m_Next->m_Engines->m_Next);
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Prev == a->m_Next);
    assert (a->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Next->m_Next->m_Engines->m_Type, "TDI 2.0"));
    assert (a->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Prev == a->m_Next->m_Next);
    assert (a->m_Next->m_Next->m_Next->m_Engines);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Next->m_Next->m_Engines->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Engines->m_Next == NULL);
    assert (a->m_Next->m_Next->m_Next->m_Next == NULL);
    a = ReorderArchive(a, LIST_BY_YEAR);
    assert (a);
    assert (a->m_Prev == NULL);
    assert (a->m_Engines);
    assert (a->m_Engines->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Type, "MPI 1.4"));
    assert (a->m_Engines->m_Prev == NULL);
    assert (a->m_Engines->m_Next->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Engines->m_Next->m_Prev == a->m_Engines);
    assert (a->m_Engines->m_Next->m_Next->m_Year == 2005);
    assert (!strcmp(a->m_Engines->m_Next->m_Next->m_Type, "TDI 2.0"));
    assert (a->m_Engines->m_Next->m_Next->m_Prev == a->m_Engines->m_Next);
    assert (a->m_Engines->m_Next->m_Next->m_Next == NULL);
    assert (a->m_Next);
    assert (a->m_Next->m_Prev == a);
    assert (a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Prev == NULL);
    assert (a->m_Next->m_Engines->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Type, "TDI 1.9"));
    assert (a->m_Next->m_Engines->m_Next->m_Prev == a->m_Next->m_Engines);
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Year == 2010);
    assert (!strcmp(a->m_Next->m_Engines->m_Next->m_Next->m_Type, "TSI 1.2"));
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Prev == a->m_Next->m_Engines->m_Next);
    assert (a->m_Next->m_Engines->m_Next->m_Next->m_Next == NULL);
    assert (a->m_Next->m_Next == NULL);
    DelArchive(a);
    return 0;
}

#endif /* __PROGTEST__ */