/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef LIST_H
#define LIST_H

typedef struct list_t List;

List* new_list(void);
void delete_list(List** list, void (* apply) (void *));

int get_size(List* list);
void append(List* list, void* value);
void* get_value(List* list, int (* condition) (void *, void *), void* arg);

void traverse_apply(List* list, void (* apply) (void *));
void traverse_2_apply(List* list, void (* apply) (void *, void *), void* arg);
void traverse_apply_back(List* list, void (* apply) (void *));
int traverse_return(List* list, int (* apply) (void *));

#endif
