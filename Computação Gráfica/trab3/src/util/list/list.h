/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */

#ifndef LIST_H
#define LIST_H

typedef struct list_t List;

List* new_list(void);
void delete_list(List** list, void (* apply) (void **));

int get_size(List* list);
void append(List* list, void* value);
void* get_value(List* list, int (* condition) (void *, void *), void* arg);
void* get_last(List* list);

void traverse(List* list, void (* apply) (void *));
void traverse_arg(List* list, void (* apply) (void *, void *), void* arg);
void traverse_back(List* list, void (* apply) (void *));
int traverse_return(List* list, int (* apply) (void *));
int traverse_true(List* list, int (* apply) (void *, void *), void* arg);

void traverse_conditional_delete(List* list, int (* condition) (void *, void *), void* arg, void (* delete) (void **));

#endif
