#if !defined(_MMA_STRING_LIST_H)
#define _MMA_STRING_LIST_H

typedef struct _StringElement StringElement;
struct _StringElement {
    StringElement   *next;
    char*           value;
};

typedef struct _StringList StringList;
struct _StringList {
    StringElement   *first;
    int             size;
};

void    StringListCreate(StringList **);
void    StringListDestroy(StringList **);
int     StringListAdd(StringList *, const char *);
int     StringListAddAll(StringList *, StringList *);
void    StringListPrint(StringList *);
int     StringListSize(StringList *);
int     StringListIndexOf(StringList *, const char *);
char*   StringListGet(StringList *, int);

#endif