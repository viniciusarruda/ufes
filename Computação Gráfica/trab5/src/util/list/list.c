/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "list.h"
#include "../util.h"


typedef struct node_t
{
	void* value;
	struct node_t* next;
} Node;


struct list_t
{
	int size;
	Node* first; 
	Node* last;
};


List* new_list(void)
{
	List* new;
	get_memory(new, sizeof(List));
	new->size = 0;
	new->first = NULL;
	new->last = NULL;
	return new;
}


int get_size(List* list)
{
	return (list == NULL) ? 0 : list->size;
}


static Node* new_node(void* value)
{
	Node* new;
	get_memory(new, sizeof(Node));
	new->value = value;
	new->next = NULL;
	return new;
}


void append(List* list, void* value)
{
	Node* node = new_node(value);

	if(list == NULL)
		raise_error("Trying to append to an unitilized list");

	if(list->first == NULL)
		list->first = node;
	else
		list->last->next = node;

	list->last = node;
	list->size++;
}


void* get_value(List* list, int (* condition) (void *, void *), void* arg)
{
	Node* tmp;
	
	if(list == NULL)
		return NULL;

	tmp = list->first;

	while(tmp != NULL)
	{	
		if(condition(tmp->value, arg))
			return tmp->value;

		tmp = tmp->next;
	}

	return NULL;
}


void* get_last(List* list)
{
	if(list == NULL)
		return NULL;

	return list->last->value;
}


void traverse(List* list, void (* apply) (void *))
{
	Node* tmp;
	
	if(list == NULL)
		return;

	tmp = list->first;

	while(tmp != NULL)
	{	
		apply(tmp->value);
		tmp = tmp->next;
	}
}


static void traverse_rec(Node* node, void (* apply) (void *))
{
	if(node == NULL)
		return;
	
	traverse_rec(node->next, apply);
	apply(node->value);	
}


void traverse_back(List* list, void (* apply) (void *)) /* traverse the list backwards */
{
	if(list == NULL)
		return;

	traverse_rec(list->first, apply);
}


void traverse_arg(List* list, void (* apply) (void *, void *), void* arg)
{
	Node* tmp;
	
	if(list == NULL)
		return;

	tmp = list->first;

	while(tmp != NULL)
	{	
		apply(tmp->value, arg);
		tmp = tmp->next;
	}
}


int traverse_return(List* list, int (* apply) (void *))
{
	Node* tmp;
	int ret;
	
	if(list == NULL)
		return -1; 

	tmp = list->first;

	while(tmp != NULL)
	{	
		if((ret = apply(tmp->value)) > 0)    
			return ret;
		tmp = tmp->next;
	}

	return -2;
}


static void shallow_delete(List** list)
{
	Node* tmp;
	while((*list)->first != NULL)
	{
		tmp = (*list)->first;
		(*list)->first = (*list)->first->next;
		tmp->value = NULL;                         
		free(tmp);
	}
}


static void deep_delete(List** list, void (* apply) (void **))
{
	Node* tmp;
	while((*list)->first != NULL)
	{
		tmp = (*list)->first;
		(*list)->first = (*list)->first->next;
		apply(&tmp->value);  
		tmp->value = NULL;                         
		free(tmp);
	}
}


void delete_list(List** list, void (* apply) (void **))
{
	if(list == NULL || *list == NULL)
		return;
	
	if(apply == NULL)
		shallow_delete(list);
	else
		deep_delete(list, apply);

	(*list)->last = NULL;
	free(*list);
	*list = NULL;
}


int traverse_true(List* list, int (* apply) (void *, void *), void* arg)
{
	Node* tmp;
	
	if(list == NULL)
		return 0; 

	tmp = list->first;

	while(tmp != NULL)
	{	
		if(apply(tmp->value, arg))    
			return 1;
		tmp = tmp->next;
	}

	return 0;
}


void traverse_conditional_delete_one(List* list, int (* condition) (void *), void (* delete) (void **))
{
	Node* previous;
	Node* hold;
	Node* next;
	
	if(list == NULL)
		return; 

	previous = NULL;
	hold = list->first;
	next = NULL;

	while(hold != NULL)
	{	
		next = hold->next;

		if(condition(hold->value))    
		{
			delete(&hold->value);
			hold->value = NULL;
			free(hold);

			if(previous == NULL)
			{
				list->first = next;
			}
			else
			{
				previous->next = next;
			}

			if(next == NULL)
			{
				list->last = previous;
			}
		}
		else
		{
			previous = hold;
		}

		hold = next;
	}
}


void traverse_conditional_delete(List* list, int (* condition) (void *, void *), void* arg, void (* delete) (void **))
{
	Node* previous;
	Node* hold;
	Node* next;
	
	if(list == NULL)
		return; 

	previous = NULL;
	hold = list->first;
	next = NULL;

	while(hold != NULL)
	{	
		next = hold->next;

		if(condition(hold->value, arg))    
		{
			delete(&hold->value);
			hold->value = NULL;
			free(hold);

			if(previous == NULL)
			{
				list->first = next;
			}
			else
			{
				previous->next = next;
			}

			if(next == NULL)
			{
				list->last = previous;
			}
		}
		else
		{
			previous = hold;
		}

		hold = next;
	}
}


int traverse_true_conditional(List* list, int (* condition) (void *, void *), void* arg_cond, int (* apply) (void *, void *), void* arg)
{
	Node* tmp;
	
	if(list == NULL)
		return 0; 

	tmp = list->first;

	while(tmp != NULL)
	{	
		if(condition(tmp->value, arg_cond) && apply(tmp->value, arg))   
			return 1;
		tmp = tmp->next;
	}

	return 0;
}


int traverse_any_all(List* list, int (* apply) (void *, void *), void* arg)
{
	Node* tmp;
	
	if(list == NULL)
		return 0; 

	tmp = list->first;

	while(tmp != NULL)
	{	
		if(apply(tmp->value, arg))   
		{
			tmp = tmp->next;
			while(tmp != NULL)
			{
				apply(tmp->value, arg);
				tmp = tmp->next;
			}
			return 1;
		}
		else
		{
			tmp = tmp->next;
		}
	}

	return 0;
}

