/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of a scanner for the language C-Minus
 *
 * Adapted to work with the second assignment (the parser)
 *
 */

%option noyywrap
%option yylineno
%option nounput
%option noinput

%{
#include "parser.h"
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

[0-9]+                   {return NUM;}
[a-zA-Z][a-zA-Z0-9]*     {return ID;}
"\""[^"\""]*"\""         {return STRING;}

"//"[^\n]*               {;}

"/*"                     {BEGIN(COMMENT);}
<COMMENT>.               {;}
<COMMENT>[\n]            {;}                        
<COMMENT>"*/"            {BEGIN(INITIAL);}

[ \t\n]                  {;}

.                        { printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext); exit(0); } 

%%
