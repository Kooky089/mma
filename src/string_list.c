#include "string_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_list_create(struct string_list** list) {
    *list = calloc(1, sizeof(struct string_list));
    (*list)->first = NULL;
    (*list)->size = 0;
}

void string_list_destroy(struct string_list** list) {
    struct string_node* current;
    struct string_node* next;
    int i;
    current = (*list)->first;
    for (i = 0; i < (*list)->size; ++i) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    free(*list);
    *list = NULL;
}

int string_list_add(struct string_list* list, const char* string) {
    struct string_node* next;
    struct string_node* current;
    int i;

    next = malloc(sizeof(struct string_node));
    if (!next) {
        return 1;
    }
    next->value = malloc(sizeof(char) * (strlen(string) + 1));
    if (!next->value) {
        free(next);
        return 1;
    }
    memcpy(next->value, string, sizeof(char) * (strlen(string) + 1));

    next->next = NULL;

    if (list->size > 0) {
        /* add after last element */
        for (i = 0; i < list->size; ++i) {
            if (!i) {
                current = list->first;
            } else {
                current = current->next;
            }
            /* string already exists in set */
            if (!strcmp(current->value, string)) {
                free(next->value);
                free(next);
                return 1;
            }
        }
        current->next = next;
        ++list->size;
    } else {
        /* add first element */
        list->first = next;
        list->size = 1;
    }
    return 0;
}

int string_list_add_all(struct string_list* list, struct string_list* list2) {
    struct string_node* current;
    int i;

    current = list2->first;
    for (i = 0; i < list2->size; ++i) {
        string_list_add(list, current->value);
        current = current->next;
    }
    return 0;
}

void string_list_print(struct string_list* list) {
    struct string_node* current;
    int i;
    printf("String List:\n");
    current = list->first;
    for (i = 0; i < list->size; ++i) {
        printf("%d: %s\n", i, current->value);
        current = current->next;
    }
}

int string_list_size(struct string_list* list) { return list->size; }

int string_list_index_of(struct string_list* list, const char* string) {
    struct string_node* current;
    int i;

    current = list->first;
    for (i = 0; i < list->size; ++i) {
        if (!strcmp(current->value, string)) {
            return i;
        }
        current = current->next;
    }
    return -1;
}

char* string_list_get(struct string_list* list, int index) {
    struct string_node* current;
    int i;
    assert(list);
    current = list->first;
    for (i = 0; i < list->size; ++i) {
        if (i == index) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}
