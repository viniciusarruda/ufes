#ifndef LIST_H
#define LIST_H

typedef struct list_t List;

List* new_list(void);
int get_size(List* list);

void append(List* list, void* value);
void delete_list(List** list, void (* apply) (void *));

void* get_value(List* list, int (* condition) (void *, void *), void* arg);
void traverse(List* list, void (* apply) (void *));

#endif
