#if !defined(__STRINGSET_H)
#define __STRINGSET_H

typedef struct _StringElement StringElement;
struct _StringElement {
  StringElement   *next;
  char            *value;
};

typedef struct _StringSet StringSet;
struct _StringSet {
  StringElement   *first;
  int             size;
};

void    StringSetCreate(StringSet **);
void    StringSetDestroy(StringSet **);
int     StringSetAdd(StringSet *, const char *);
int     StringSetAddAll(StringSet *, StringSet *);
void    StringSetPrint(StringSet *);
int     StringSetSize(StringSet *);
int     StringSetIndexOf(StringSet *, char const *);
char*   StringSetGet(StringSet *, int);

#endif