/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#include "../util/list.h"
#include "../util/get_memory.h"
#include "../util/error.h"
#include "../ast/ast.h"
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
			int offset;
		} scalar;

		struct
		{
			int scope;
			int size;
			int offset;
		} array;

		struct
		{
			Return_t return_type;
			List* params;
			Node* exec_point;
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


static const char* ret2str(Return_t type)
{
	static const char* ret_str[] = {"void", "int"};
	return ret_str[type];
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
	return (increment) ? func_id++ : func_id;
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


void assert_expected_return(Symbol* symbol, Return_t type)
{
	if(symbol->info.function.return_type != type)  
	{
		printf("INTERPRETER ERROR: expected %s as return, but function '%s' as declared at line %d will return %s.\n", ret2str(type), symbol->symbol, symbol->lineno, ret2str(symbol->info.function.return_type));
		exit(0);
	}
}


void assert_given_return(Symbol* symbol, Return_t type)
{
	if(symbol->info.function.return_type != type)  
	{
		printf("INTERPRETER ERROR: function '%s' returned %s but was expected to return %s as declared at line %d.\n", symbol->symbol, ret2str(type), ret2str(symbol->info.function.return_type), symbol->lineno);
		exit(0);
	}
}


void assert_lval_scalar(Symbol* symbol)
{
	if(symbol->type != SCALAR_S)                  
	{
		printf("INTERPRETER ERROR: array '%s' declared at line %d is missing access index.\n", symbol->symbol, symbol->lineno);
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
	new->info.function.exec_point = NULL;

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


void set_offset(Symbol* symbol, int offset)
{
	if(symbol->type == SCALAR_S)
		symbol->info.scalar.offset = offset;
    else // ARRAY_S
		symbol->info.array.offset = offset;
}


int get_offset(Symbol* symbol)
{
	if(symbol->type == SCALAR_S)
		return symbol->info.scalar.offset;
    else // ARRAY_S
		return symbol->info.array.offset;
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


inline int get_array_size(Symbol* symbol)
{
	return symbol->info.array.size;
}


inline int is_array(Symbol* symbol)
{
	return symbol->type == ARRAY_S;
}


inline const char* look_up_literal(Literal* literal)
{
	return literal->literal;
}


inline List* get_params(Symbol* symbol)
{
	return symbol->info.function.params;
}


inline int has_return(Symbol* symbol)
{
	return symbol->info.function.return_type != VOID_R;
}


inline void set_exec_point(Symbol* symbol, Node* node)
{
	symbol->info.function.exec_point = node;
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


Node** get_function(Table* table, const char* symbol, int length_args, int lineno)    
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

	return &s->info.function.exec_point; 
}


Node** get_main(Table* table)    
{
	int pp;
	Symbol* s = get_value(table->symbols, equals_function, (void *) "main");

	if(s == NULL) 
	{
		printf("INTERPRETER ERROR: main function not declared.\n");
		exit(0);
	}

	pp = get_size(s->info.function.params);

	if(pp != 0)
	{
		printf("INTERPRETER ERROR: main function was declared at line %d with %d parameters but should be void.\n", s->lineno, pp);
		exit(0);
	}

	if(s->info.function.return_type != VOID_R)
	{
		printf("INTERPRETER ERROR: main function was declared at line %d with %s as return value but should return void.\n", s->lineno, ret2str(s->info.function.return_type));
		exit(0);
	}

	return &s->info.function.exec_point; 
}
