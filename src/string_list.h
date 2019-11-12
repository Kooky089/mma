#if !defined(MMA_STRING_LIST_H)
#define MMA_STRING_LIST_H

struct string_node {
    struct string_node      *next;
    char                    *value;
};

struct string_list {
    struct string_node      *first;
    int                     size;
};

void    string_list_create(struct string_list **);
void    string_list_destroy(struct string_list **);
int     string_list_add(struct string_list *, const char *);
int     string_list_add_all(struct string_list *, struct string_list *);
void    string_list_print(struct string_list *);
int     string_list_size(struct string_list *);
int     string_list_index_of(struct string_list *, const char *);
char   *string_list_get(struct string_list *, int);

#endif
