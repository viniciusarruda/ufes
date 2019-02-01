#include "ast.h"
#include "../util/get_memory.h"
#include "../util/list.h"
#include "../table/table.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

struct node_t
{
	Ast_t type;
	void** value;
};

static int child_size[] = {3, 1, 2, 2, 2, 3, 2, 1, 1, 1, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};


Node* new_node(Ast_t type, ...)
{
	int i;
	Node* new;
	va_list ap;

	get_memory(new, sizeof(Node));
	new->type = type;

	if(type == FUNCTION_T)
		increment_func_id();

	get_memory(new->value, sizeof(void *) * child_size[type]);
    
    va_start(ap, type);
    for (i = 0; i < child_size[type]; i++) 
    	new->value[i] = va_arg(ap, void *);
    va_end(ap);

	return new;
}


static void delete_node(void* n)
{
	Node* node = n;
	free(node->value);
	free(node);
}


void delete_ast(void* n)
{
	Node* node = n;

	switch(node->type)
	{
		case FUNCTION_T:
			delete_list((List **) &node->value[1], NULL);
			delete_list((List **) &node->value[2], delete_ast);
			break;
		case LVAL_T: case LVAL_ID_T: case WRITE_T: case INPUT_T:
			break;
		case LVAL_NUM_T:
			free(node->value[1]);
			break;
		case ASSIGN_T:
			delete_ast(node->value[0]);
			delete_ast(node->value[1]);
			break;
		case IF_T:
			delete_ast(node->value[0]);
			delete_list((List **) &node->value[1], delete_ast);
			delete_list((List **) &node->value[2], delete_ast);
			break;	
		case WHILE_T:
			delete_ast(node->value[0]);	// without break 
		case FCALL_T:
			delete_list((List **) &node->value[1], delete_ast);
			break;
		case RETURN_T: 
			if(node->value[0] != NULL)
				delete_ast(node->value[0]);
			break;	
		case OUTPUT_T: 
			delete_ast(node->value[0]);
			break;		
		case NUM_T:
			free(node->value[0]);
			break;	
		case LT_T: case LE_T: case GT_T: case GE_T: case EQ_T:
		case NEQ_T: case PLUS_T: case MINUS_T: case TIMES_T: case OVER_T:
			delete_ast(node->value[0]);
			delete_ast(node->value[1]);
			break;			
		default:
			printf("ERROR !!!!");
			exit(0);
	}

	delete_node(node);
}

