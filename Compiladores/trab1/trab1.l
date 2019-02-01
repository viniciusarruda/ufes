/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Eduardo Zambon
 * Subject: Compilers
 * Assignment: Implementation of a scanner for the language C-Minus
 */

%option noyywrap
%option yylineno
%option nounput
%option noinput

%{

#include <stdio.h>  

void show(const char* tokenType)
{
    printf("%d: %s -> %s\n", yylineno, yytext, tokenType);
}

%}

%x COMMENT
%%

"else"   {show("ELSE");}
"if"     {show("IF");}
"input"  {show("INPUT");}
"int"    {show("INT");}
"output" {show("OUTPUT");}
"return" {show("RETURN");}
"void"   {show("VOID");}
"while"  {show("WHILE");}
"write"  {show("WRITE");}

"+"      {show("PLUS");}
"-"      {show("MINUS");}
"*"      {show("TIMES");}
"/"      {show("OVER");}
"<"      {show("LT");}
"<="     {show("LE");}
">"      {show("GT");}
">="     {show("GE");}
"=="     {show("EQ");}
"!="     {show("NEQ");}
"="      {show("ASSIGN");}

";"      {show("SEMI");}
","      {show("COMMA");}
"("      {show("LPAREN");}
")"      {show("RPAREN");}
"["      {show("LBRACK");}
"]"      {show("RBRACK");}
"{"      {show("LBRACE");}
"}"      {show("RBRACE");}

[0-9]+                   {show("NUM");}
[a-zA-Z][a-zA-Z0-9]*     {show("ID");}
"\""[^"\""]*"\""         {show("STRING");}

"//"[^\n]*               {;}

"/*"                     {BEGIN(COMMENT);}
<COMMENT>.               {;}
<COMMENT>[\n]            {;}                        
<COMMENT>"*/"            {BEGIN(INITIAL);}

[ \t\n]                  {;}

.                        {show("UNKNOWN");}

%%

int main() 
{ 
	yylex(); 
	return 0; 
}
