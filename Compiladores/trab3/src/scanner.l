/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of a scanner for the language C-Minus
 *
 * Adapted to work with the second assignment (the parser)
 * Adapted to work with the third assignment (ast and table of symbols and literals)
 *
 */

%option noyywrap
%option yylineno
%option nounput
%option noinput

%{
#include "parser.h"
#include "util/get_memory.h"
#include <string.h>
%}

%x COMMENT
%%

"else"   {return ELSE;}
"if"     {return IF;}
"input"  {return INPUT;}
"int"    {return INT;}
"output" {return OUTPUT;}
"return" {return RETURN;}
"void"   {return VOID;}
"while"  {return WHILE;}
"write"  {return WRITE;}

"+"      {return PLUS;}
"-"      {return MINUS;}
"*"      {return TIMES;}
"/"      {return OVER;}
"<"      {return LT;}
"<="     {return LE;}
">"      {return GT;}
">="     {return GE;}
"=="     {return EQ;}
"!="     {return NEQ;}
"="      {return ASSIGN;}

";"      {return SEMI;}
","      {return COMMA;}
"("      {return LPAREN;}
")"      {return RPAREN;}
"["      {return LBRACK;}
"]"      {return RBRACK;}
"{"      {return LBRACE;}
"}"      {return RBRACE;}
                                  
[0-9]+                   {int* num; get_memory(num, sizeof(int)); *num = atoi(yytext); yylval = num; return NUM;}  /* for while, forget the table !!! */
[a-zA-Z][a-zA-Z0-9]*     {yylval = strdup(yytext); return ID;}  /* strdup because yytext is a buffer pointer ! and will change ! */
"\""[^"\""]*"\""         {yylval = strdup(yytext); return STRING;}

"//"[^\n]*               {;}

"/*"                     {BEGIN(COMMENT);}
<COMMENT>.               {;}
<COMMENT>[\n]            {;}                        
<COMMENT>"*/"            {BEGIN(INITIAL);}

[ \t\n]                  {;}

.                        { printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext); exit(0); } 

%%
