/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of an interpreter for the language C-Minus
 *
 * Adapted to work with the third assignment (ast and table of symbols and literals)
 * Adapted to work with the fourth assignment (interpreter)
 *
 */


%defines "src/parser.h"
%define parse.error verbose
%define parse.lac full
%define api.value.type {void *}

%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast/ast.h"
#include "util/list.h"
#include "table/table.h"
#include "interpreter/interpreter.h" 

List* func_list = NULL;
void* holder = NULL;
Return_t return_type;

Table* table = NULL;

int yylex();
void yyerror(const char *s);

extern int yylineno;
%}

%token ELSE IF INPUT INT OUTPUT RETURN VOID WHILE WRITE
%token SEMI LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token COMMA ASSIGN EQ NEQ LT LE GT GE
%token NUM ID STRING 

%left PLUS MINUS
%left TIMES OVER


%start program                        

%%

program:
  func_decl_list                               {func_list = $1;}                  
;

func_decl_list:
  func_decl_list func_decl                     {append($1, $2); $$ = $1;}
| func_decl                                    {holder = new_list(); append(holder, $1); $$ = holder;} 
;

func_decl:
  func_header LBRACE opt_var_decl opt_stmt_list RBRACE  {$$ = new_node(FUNCTION_T, $1, $3, $4);}
;

func_header:
  ret_type ID LPAREN params RPAREN             {$$ = add_function(table, *((Return_t *) $1), $2, $4, yylineno);}
;

opt_var_decl:
  %empty                                    {$$ = NULL;}
| var_decl_list                             {$$ = $1;}
;

opt_stmt_list:
  %empty                                    {$$ = NULL;}
| stmt_list                                 {$$ = $1;}
;

ret_type:
  INT                                       {return_type = INT_R; $$ = &return_type;}
| VOID                                      {return_type = VOID_R; $$ = &return_type;}
;

params:
  VOID                                      {$$ = NULL;}
| param_list                                {$$ = $1;}  
;

param_list:
  param_list COMMA param                    {append($1, $3); $$ = $1;}
| param                                     {holder = new_list(); append(holder, $1); $$ = holder;} 
;

param:
  INT ID                                    {$$ = add_scalar(table, $2, yylineno);}  
| INT ID LBRACK RBRACK                      {$$ = add_array(table, $2, yylineno, -1);} 
;

var_decl_list:
  var_decl_list var_decl                    {append($1, $2); $$ = $1;}
| var_decl                                  {holder = new_list(); append(holder, $1); $$ = holder;} 
;

var_decl:
  INT ID SEMI                               {$$ = add_scalar(table, $2, yylineno);}                   
| INT ID LBRACK NUM RBRACK SEMI             {int* num = $4; $$ = add_array(table, $2, yylineno, *num); free(num);}
;

stmt_list:
  stmt_list stmt                            {append($1, $2); $$ = $1;}
| stmt                                      {holder = new_list(); append(holder, $1); $$ = holder;} 
;

stmt:
  assign_stmt                               {$$ = $1;}
| if_stmt                                   {$$ = $1;}
| while_stmt                                {$$ = $1;}
| return_stmt                               {$$ = $1;}
| func_call SEMI                            {$$ = $1;}
;

assign_stmt:                            
  lval ASSIGN arith_expr SEMI               {$$ = new_node(ASSIGN_T, $1, $3);} 
;

lval:
  ID                            {$$ = new_node(LVAL_T, get_variable(table, $1, yylineno), NULL); free($1);}        
| ID LBRACK NUM RBRACK          {$$ = new_node(LVAL_NUM_T, get_variable(table, $1, yylineno), $3); free($1);}       
| ID LBRACK ID RBRACK           {$$ = new_node(LVAL_ID_T, get_variable(table, $1, yylineno), get_variable(table, $3, yylineno));free($1);free($3);}  
;

if_stmt:
  IF LPAREN bool_expr RPAREN block                  {$$ = new_node(IF_T, $3, $5, NULL);} 
| IF LPAREN bool_expr RPAREN block ELSE block       {$$ = new_node(IF_T, $3, $5, $7);} 
;

block:
  LBRACE opt_stmt_list RBRACE               {$$ = $2;}
;

while_stmt:
  WHILE LPAREN bool_expr RPAREN block       {$$ = new_node(WHILE_T, $3, $5);}
;

return_stmt:
  RETURN SEMI                               {$$ = new_node(RETURN_T, NULL);}
| RETURN arith_expr SEMI                    {$$ = new_node(RETURN_T, $2);}
;

func_call:
  output_call                               {$$ = $1;}
| write_call                                {$$ = $1;}
| user_func_call                            {$$ = $1;}
;

input_call:
  INPUT LPAREN RPAREN                       {$$ = new_node(INPUT_T);}         
;

output_call:
  OUTPUT LPAREN arith_expr RPAREN           {$$ = new_node(OUTPUT_T, $3);}  
;

write_call:
  WRITE LPAREN STRING RPAREN                {$$ = new_node(WRITE_T, add_literal(table, $3));}
;

user_func_call:
  ID LPAREN opt_arg_list RPAREN             {$$ = new_node(FCALL_T, get_function(table, $1, get_size($3), yylineno), $3);free($1);}
;

opt_arg_list:
  %empty                                    {$$ = NULL;}
| arg_list                                  {$$ = $1;}                          
;

arg_list:
  arg_list COMMA arith_expr                 {append($1, $3); $$ = $1;}
| arith_expr                                {holder = new_list(); append(holder, $1); $$ = holder;}      
;

bool_expr:
  arith_expr LT arith_expr                  {$$ = new_node(LT_T, $1, $3);}
| arith_expr LE arith_expr                  {$$ = new_node(LE_T, $1, $3);}
| arith_expr GT arith_expr                  {$$ = new_node(GT_T, $1, $3);}   
| arith_expr GE arith_expr                  {$$ = new_node(GE_T, $1, $3);}
| arith_expr EQ arith_expr                  {$$ = new_node(EQ_T, $1, $3);}
| arith_expr NEQ arith_expr                 {$$ = new_node(NEQ_T, $1, $3);}
;

arith_expr:
  arith_expr PLUS arith_expr          {$$ = new_node(PLUS_T, $1, $3);}
| arith_expr MINUS arith_expr         {$$ = new_node(MINUS_T, $1, $3);}
| arith_expr TIMES arith_expr         {$$ = new_node(TIMES_T, $1, $3);}
| arith_expr OVER arith_expr          {$$ = new_node(OVER_T, $1, $3);}
| LPAREN arith_expr RPAREN            {$$ = $2;}
| lval                                {$$ = $1;}
| input_call                          {$$ = $1;}
| user_func_call                      {$$ = $1;}
| NUM                                 {$$ = new_node(NUM_T, $1);}                       
;




%%


void yyerror (char const *s) 
{
	printf("PARSE ERROR (%d): %s\n", yylineno, s);
}


int main() 
{
	table = new_table();
    int ret = yyparse();

	if(ret == 0) 
	{
        if(freopen(ctermid(NULL), "r", stdin) == NULL) 
        {
            perror(ctermid(NULL));
            exit(0);
        }

        run_main(table);

        fclose(stdin);
		delete_list(&func_list, delete_ast);
  		delete_table(table);

        /* yylex_destroy();  see README */
	}

	return 0;
}

