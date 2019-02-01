/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef TABLE_H
#define TABLE_H

#include "../util/list.h"
#include "../ast/ast.h"

typedef enum {VOID_R, INT_R} Return_t;
typedef enum {SCALAR_S, ARRAY_S, FUNCTION_S} Symbol_t;

typedef struct symbol_t Symbol;
typedef struct table_t Table;
typedef struct literal_t Literal;

Table* new_table(void);
void delete_table(Table* t);
void delete_symbol(void* s);
void delete_literal(void* l);

Symbol* add_function(Table* table, Return_t return_type, char const* symbol, List* params, int lineno);
Symbol* add_scalar(Table* table, char const* symbol, int lineno);
Symbol* add_array(Table* table, char const* symbol, int lineno, int size);
Literal* add_literal(Table* table, char const* literal);

Node** get_function(Table* table, const char* symbol, int length_args, int lineno);
Node** get_main(Table* table);
Symbol* get_variable(Table* table, const char* symbol, int lineno);
void increment_func_id(void);

void set_offset(Symbol* symbol, int offset);
int get_offset(Symbol* symbol);

inline void set_exec_point(Symbol* symbol, Node* node);
inline const char* look_up_literal(Literal* literal);
inline List* get_params(Symbol* symbol);
inline int has_return(Symbol* symbol);
inline int is_array(Symbol* symbol);
inline int get_array_size(Symbol* symbol);

void assert_expected_return(Symbol* symbol, Return_t type);
void assert_given_return(Symbol* symbol, Return_t type);
void assert_lval_scalar(Symbol* symbol);

#endif
