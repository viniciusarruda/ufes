/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an ast and table of symbols and literals for the language C-Minus
 *
 */

#include "../util/list.h"
#include "../util/get_memory.h"
#include "table.h"
#include <string.h>

struct symbol_t
{
	Symbol_t type;
	int lineno;
	const char* symbol;
	int id;
	
	union
	{
		struct
		{
			int scope;
		} scalar;

		struct
		{
			int scope;
			int size;
		} array;

		struct
		{
			Return_t return_type;
			List* params;
		} function;
	} info;
};


struct literal_t
{
	int id;
	const char* literal;
};


struct table_t
{
	List* symbols;
	List* literals;
};                        


Table* new_table(void)
{
	Table* new;
	get_memory(new, sizeof(Table));

	new->symbols = new_list();
	new->literals = new_list();                    

	return new;
}


void delete_table(Table* t)
{
	if(t == NULL) return;

	delete_list(&t->symbols, delete_symbol);
	delete_list(&t->literals, delete_literal);
	free(t);
}


static Symbol* new_symbol(Symbol_t type, const char* symbol, int id, int lineno)
{
	Symbol* new;
	get_memory(new, sizeof(Symbol));

	new->type = type;
	new->symbol = symbol;   
	new->id = id;
	new->lineno = lineno;   

	return new;
}


void delete_symbol(void* s)
{
	if(s == NULL) return;

	free( (char *) ((Symbol *)s)->symbol);

	if(((Symbol *)s)->type == FUNCTION_S)
		delete_list(&((Symbol *)s)->info.function.params, NULL);

	free(s);
}


static Literal* new_literal(int id, const char* literal)
{
	Literal* new;
	get_memory(new, sizeof(Literal));

	new->id = id;
	new->literal = literal;   

	return new;
}


void delete_literal(void* l)
{
	if(l == NULL) return;

	free( (char *) ((Literal *)l)->literal);
	free(l);
}


static int get_func_id(int increment)
{
	static int func_id = 0; 
	
	if(increment)
		return func_id++;	
	else
		return func_id;
}


void increment_func_id(void)
{
	get_func_id(1);
}


static int get_var_id(void)
{
	static int var_id = 0; 
	return var_id++;	
}


static int equals_variable(void* value, void* str)
{
	if(((Symbol *) value)->type == SCALAR_S)
	{
		return strcmp(((Symbol *) value)->symbol, (const char *) str) == 0 && 
		       ((Symbol *) value)->info.scalar.scope == get_func_id(0);
	}
	else if(((Symbol *) value)->type == ARRAY_S)
	{
		return strcmp(((Symbol *) value)->symbol, (const char *) str) == 0 && 
		       ((Symbol *) value)->info.array.scope == get_func_id(0);
	}

	return 0; 
}


static int equals_function(void* value, void* str)
{
	if(((Symbol *) value)->type == FUNCTION_S)
		return strcmp(((Symbol *) value)->symbol, (const char *) str) == 0;

	return 0;
}


static int equals_literal(void* value, void* str)
{
	return strcmp(((Literal *) value)->literal, (const char *) str) == 0;
}


static Literal* get_literal(Table* table, const char* literal)
{
	return get_value(table->literals, equals_literal, (void *) literal);
}


static void assert_unique_variable(Table* table, const char* symbol, int lineno)
{
	Symbol* s = get_value(table->symbols, equals_variable, (void *) symbol);

	if(s != NULL)
	{
		printf("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n", lineno, symbol, s->lineno);
		exit(0);
	} 
}


static void assert_unique_function(Table* table, const char* symbol, int lineno)
{
	Symbol* s = get_value(table->symbols, equals_function, (void *) symbol);

	if(s != NULL)
	{
		printf("SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n", lineno, symbol, s->lineno);
		exit(0);
	}
}


Symbol* add_function(Table* table, Return_t return_type, const char* symbol, List* params, int lineno) 
{
	Symbol* new;

	assert_unique_function(table, symbol, lineno); 

	new = new_symbol(FUNCTION_S, symbol, get_func_id(0), lineno);
	new->info.function.return_type = return_type;
	new->info.function.params = params;

	append(table->symbols, new);                   

	return new;
}


Symbol* add_scalar(Table* table, const char* symbol, int lineno)
{
	Symbol* new;

	assert_unique_variable(table, symbol, lineno); 

	new = new_symbol(SCALAR_S, symbol, get_var_id(), lineno);
	new->info.scalar.scope = get_func_id(0);

	append(table->symbols, new);              

	return new;
}


Symbol* add_array(Table* table, const char* symbol, int lineno, int size)
{
	Symbol* new;

	assert_unique_variable(table, symbol, lineno); 

	new = new_symbol(ARRAY_S, symbol, get_var_id(), lineno);
	new->info.array.scope = get_func_id(0);
	new->info.array.size = size;

	append(table->symbols, new);   

	return new;          
}


Literal* add_literal(Table* table, const char* literal) 
{
	static int literal_id = 0;
	Literal* new;

	if((new = get_literal(table, literal)) != NULL)
	{
		free((char *) literal);
		return new;
	}

	new = new_literal(literal_id++, literal);

	append(table->literals, new);

	return new;
}


Symbol* get_variable(Table* table, const char* symbol, int lineno)    
{
	Symbol* s = get_value(table->symbols, equals_variable, (void *) symbol);

	if(s == NULL)
	{
		printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n", lineno, symbol); 
		exit(0);
	}

	return s; 
}

Symbol* get_function(Table* table, const char* symbol, int length_args, int lineno)    
{
	int pp;
	Symbol* s = get_value(table->symbols, equals_function, (void *) symbol);

	if(s == NULL) 
	{
		printf("SEMANTIC ERROR (%d): function '%s' was not declared.\n", lineno, symbol);
		exit(0);
	}

	pp = get_size(s->info.function.params);

	if(pp != length_args)
	{
		printf("SEMANTIC ERROR (%d): function '%s' was called with %d arguments but declared with %d parameters.\n", lineno, symbol, length_args, pp);
		exit(0);
	}

	return s; 
}
