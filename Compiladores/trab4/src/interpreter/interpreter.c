/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "../table/table.h"
#include "stack.h"
#include "../util/error.h"
#include "../ast/ast.h"

#define DEFAULT_VALUE 0
#define RETURN_INT 2   
#define RETURN_VOID 1  
#define NO_RETURN 0    
  
static void run_rec_ast(void* n);


static void declare_variable(void* symbol)  
{
    if(is_array(symbol))
    {
        set_offset(symbol, push(get_next_address() + 1));   /* the offset (address) of initial array data */
        push_times(DEFAULT_VALUE, get_array_size(symbol));  /* the values it self                         */
    }
    else
    {
        set_offset(symbol, push(DEFAULT_VALUE));
    }
}


static void remove_variable(void* symbol)  
{
    if(is_array(symbol))
        pop_times(get_array_size(symbol) + 1);              /* +1 to remove address */
    else
        pop();
}


static void declare_variable_arguments(void* symbol, void* offset) 
{
    if(is_array(symbol))
        set_offset(symbol, load((*((int *)offset))++) - get_fp() - 1);  /* get the reference */
    else
        set_offset(symbol, (*((int *)offset))++);
}


static int get_int(void)
{
    long val;
    char* endptr;
    char* line = NULL;
    size_t n = 0;

   if(getline(&line, &n, stdin) < 1)
        raise_error("INTERPRETER ERROR: expected input, got nothing.");

   val = strtol(line, &endptr, 10);

   if(endptr == line || (*endptr != '\0' && *endptr != '\n'))   
        raise_error("INTERPRETER ERROR: expected int as input, got string.");

   free(line);

   return (int) val; /* may occur overflow or underflow */
}


static void write(const char* str)  /* to support newline */
{
    for(; *str != '\0'; str++)
        putc((*str == '\\' && *(str+1) == 'n') ? (str++,'\n') : *str, stdout);
}


static int run_rec_stmt(void* _node)  /* patience, I know it's ugly */
{
    Node* node = _node; 
    int tmp, ret = NO_RETURN;

    switch(get_type(node))
    {
        case FUNCTION_T:
            push_fp();
            tmp = -get_size(get_params(get_child(node, 0)));    /* the length of parameters will be used to set the offset where the arguments pushed by FCALL_T will be */
            traverse_2_apply(get_params(get_child(node, 0)), declare_variable_arguments, &tmp); 
            traverse_apply(get_child(node, 1), declare_variable);
            ret = traverse_return(get_child(node, 2), run_rec_stmt);
            if(ret == RETURN_INT)
            {
                assert_given_return(get_child(node, 0), INT_R);
                store(-(get_size(get_params(get_child(node, 0))) + 1), pop()); /* stores returned type to its address */
            }
            else
            {   
                assert_given_return(get_child(node, 0), VOID_R);
            }
            traverse_apply_back(get_child(node, 1), remove_variable);
            pop_fp();
            break;
        case IF_T:
            run_rec_ast(get_child(node, 0));   
            if(pop())
                ret = traverse_return(get_child(node, 1), run_rec_stmt);
            else if(get_child(node, 2) != NULL)
                ret = traverse_return(get_child(node, 2), run_rec_stmt);
            break;
        case WHILE_T:
            while(run_rec_ast(get_child(node, 0)), pop())
                ret = traverse_return(get_child(node, 1), run_rec_stmt);
            break;
        case RETURN_T:
            ret = (get_child(node, 0) != NULL) ? run_rec_ast(get_child(node, 0)), RETURN_INT : RETURN_VOID;
            break;
        case FCALL_T:
            run_rec_ast(node);
            if(has_return(get_child(get_exec_point(node), 0)))
                pop();                                             /* pops unused return of function */
            break;
        default:
            run_rec_ast(node);
            break;
    }

    return ret;
}


static int get_offset_child(Node* node)
{
    switch(get_type(node))
    {
        case LVAL_T:     return get_offset(get_child(node, 0));   
        case LVAL_NUM_T: return get_offset(get_child(node, 0)) + 1 + *((int *)get_child(node, 1));    
        case LVAL_ID_T:  return get_offset(get_child(node, 0)) + 1 + load(get_offset(get_child(node, 1)));
        default:         raise_error(IMPLEMENTATION_ERROR);
    }
}


static void run_rec_ast(void* _node)  /* a little bit more patience... */
{
    Node* node = _node; 
    int a, b;

    switch(get_type(node))
    {
        case ASSIGN_T:
            if(get_type(get_child(node, 1)) == FCALL_T) 
                assert_expected_return(get_child(get_exec_point(get_child(node, 1)), 0), INT_R);   
            if(get_type(get_child(node, 1)) == LVAL_T) 
                assert_lval_scalar(get_child(get_child(node, 1), 0));
            if(get_type(get_child(node, 0)) == LVAL_T) 
                assert_lval_scalar(get_child(get_child(node, 0), 0));
            run_rec_ast(get_child(node, 1));
            store(get_offset_child(get_child(node, 0)), pop());
            break;
        case FCALL_T:
            if(has_return(get_child(get_exec_point(node), 0)))   
                push(DEFAULT_VALUE);                          /* return value will be stored here */
            traverse_apply(get_child(node, 1), run_rec_ast);  /* stacks arguments in order        */
            run_rec_stmt(get_exec_point(node));               /* run function                     */
            pop_times(get_size(get_child(node, 1)));          /* pops arguments in order          */
            break;
        case OUTPUT_T:
            if(get_type(get_child(node, 0)) == LVAL_T) 
                assert_lval_scalar(get_child(get_child(node, 0), 0));
            run_rec_ast(get_child(node, 0));
            printf("%d", pop());   
            break;
        case LVAL_T: case LVAL_NUM_T: case LVAL_ID_T:  push(load(get_offset_child(node))); break;
        case WRITE_T: write(look_up_literal(get_child(node, 0)));                          break;
        case INPUT_T: 
            printf("input: "); 
            push(get_int());                                                     
            break;
        case NUM_T:   push(*((int*)get_child(node, 0)));                                   break;
        default:
            run_rec_ast(get_child(node, 0));
            run_rec_ast(get_child(node, 1));
            b = pop();
            a = pop();
            switch(get_type(node))
            {
                case LT_T:    push(a < b);  break;
                case LE_T:    push(a <= b); break;
                case GT_T:    push(a > b);  break;
                case GE_T:    push(a >= b); break;
                case EQ_T:    push(a == b); break;
                case NEQ_T:   push(a != b); break;
                case PLUS_T:  push(a + b);  break;     /* may occur overflow  */
                case MINUS_T: push(a - b);  break;     /* may occur underflow */
                case TIMES_T: push(a * b);  break;     /* may occur overflow  */
                case OVER_T:  push(a / b);  break;
                default:      raise_error(IMPLEMENTATION_ERROR);
            }
    }
}


void run_main(Table* table)
{
    new_stack();
    run_rec_stmt(*get_main(table));
    delete_stack();
}
