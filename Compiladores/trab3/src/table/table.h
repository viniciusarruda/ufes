/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an ast and table of symbols and literals for the language C-Minus
 *
 */

#ifndef TABLE_H
#define TABLE_H

#include "../util/list.h"

typedef enum {SCALAR_S, ARRAY_S, FUNCTION_S} Symbol_t;
typedef enum {VOID_R, INT_R} Return_t;

typedef struct symbol_t Symbol;
typedef struct table_t Table;
typedef struct literal_t Literal;

Table* new_table(void);
Symbol* add_function(Table* table, Return_t return_type, char const* symbol, List* params, int lineno);
Symbol* add_scalar(Table* table, char const* symbol, int lineno);
Symbol* add_array(Table* table, char const* symbol, int lineno, int size);
Literal* add_literal(Table* table, char const* literal);
Symbol* get_function(Table* table, const char* symbol, int length_args, int lineno);
Symbol* get_variable(Table* table, const char* symbol, int lineno);
void increment_func_id(void);

void delete_table(Table* t);
void delete_symbol(void* s);
void delete_literal(void* l);

#endif
