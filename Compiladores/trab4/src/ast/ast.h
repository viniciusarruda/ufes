/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#ifndef AST_H
#define AST_H

#include <stdarg.h>

typedef enum 
{
    FUNCTION_T,
    LVAL_T,
    LVAL_NUM_T,
    LVAL_ID_T,
    ASSIGN_T, 
    IF_T, 
    WHILE_T, 
    RETURN_T, 
    OUTPUT_T, 
    WRITE_T, 
    FCALL_T,
    INPUT_T, 
    NUM_T,
    LT_T,
    LE_T,
    GT_T,
    GE_T,
    EQ_T,
    NEQ_T,
    PLUS_T,
    MINUS_T,
    TIMES_T,
    OVER_T
} Ast_t;

typedef struct node_t Node;

Node* new_node(Ast_t type, ...);
void delete_ast(void* node);

inline Ast_t get_type(Node* node);
inline void* get_child(Node* node, int index);

inline Node* get_exec_point(Node* node);

#endif
