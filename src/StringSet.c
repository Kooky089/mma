#include"StringSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void StringSetCreate(StringSet **list) {
    *list = calloc(1,sizeof(StringSet));
    (*list)->first = NULL;
    (*list)->size = 0;
}

void StringSetDestroy(StringSet **list) {
    StringElement *current;
    StringElement *next;
    int i;
    current = (*list)->first;
    for (i=0; i<(*list)->size; i++) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    free(*list);
    *list = NULL;
}

int StringSetAdd(StringSet *list, const char *string) {
    StringElement *new;
    StringElement *current;
    int i;

    new = malloc(sizeof(StringElement));
    new->value = malloc(sizeof(char)*(strlen(string)+1));
    memcpy(new->value,string,sizeof(char)*(strlen(string)+1));
    new->next = NULL;

    if (list->size > 0) {
        /* add after last element */
        for (i=0; i < list->size; i++) {
            if (!i) {
                current = list->first;
            } else {
                current = current->next;
            }
            /* string already exists in set */
            if (!strcmp(current->value,string)) {
                return 1;
            }
        }
        current->next = new;
        list->size++;
    } else {
        /* add first element */
        list->first = new;
        list->size = 1;
    }
    return 0;
}

int StringSetAddAll(StringSet *list, StringSet *list2) {
    StringElement *current;
    int i;

    current = list2->first;
    for (i=0; i<list2->size; i++) {
        StringSetAdd(list,current->value);
        current = current->next;
    }
    return 0;
}

void StringSetPrint(StringSet *list) {
    StringElement *current;
    int i;
    printf("String Set:\n");
    current = list->first;
    for (i=0; i<list->size; i++) {
        printf("%d: %s\n",i,current->value);
        current = current->next;
    }
}

int StringSetSize(StringSet *list) {
    return list->size;
}

int StringSetIndexOf(StringSet *list, char const *string) {
    StringElement *current;
    int i;

    current = list->first;
    for (i=0; i<list->size; i++) {
        if (!strcmp(current->value,string)) {
            return i;
        }
        current = current->next;
    }
    return -1;
}

char* StringSetGet(StringSet *list, int index) {
    StringElement *current;
    int i;
    current = list->first;
    for (i=0; i<list->size; i++) {
        if (i==index) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}
