
%{
#include "globals.h"
#include "util.h"
#include "scan.h"
char g_tokenString[MAXTOKENLEN + 1] = "";  // lexeme.
%}
digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
whitespace  [ \t]+

%%

"if"        {return IF;}
"else"      {return ELSE;}
"int"       {return INT;}
"void"      {return VOID;}
"while"     {return WHILE;}
"return"    {return RETURN;}
"="         {return ASSIGN;}
";"         {return SEMI;}
","         {return COMMA;}
"+"         {return PLUS;}
"-"         {return MINUS;}
"*"         {return MUL;}
"/"         {return DIV;}
"<"         {return LT;}
"<="        {return LET;}
">"         {return GT;}
">="        {return GET;}
"=="        {return EQ;}
"!="        {return NEQ;}
"("         {return LPRTS;}
")"         {return RPRTS;}
"["         {return LBRKT;}
"]"         {return RBRKT;}
"{"         {return LBRC;}
"}"         {return RBRC;}
\n          {++g_line;}
{number}        {return NUM;}
{identifier}    {return ID;}
{whitespace}    {/*ignore*/}
"/*"            {
                    char c, state = 0;
                    // final state is 2.
                    while (state != 2) {
                        // get edge.
                        c = input();
                        if (c == '\n')
                            ++g_line;
                        else if (c == EOF)
                            return ERROR;
                        // state transition.
                        switch (state) {
                            case 0:
                                state = (c == '*')? 1 : 0;
                                break;
                            case 1:
                                state = (c == '*')? 1 : ((c == '/')? 2 : 0);
                                break;
                        }
                    }
                }               
.           {return ERROR;}

%%

int yywrap(void) {
    return 1;
}

TokenType getToken(void) {
    
    static int init = TRUE;
    TokenType currentToken;

    if (init) {
        init  = FALSE;
        yyin  = g_src;
        yyout = g_dest;
        ++g_line;
    }

    currentToken = yylex();
    if (strcmp(yytext, ""))
        strncpy(g_tokenString, yytext, MAXTOKENLEN);
    else
        strcpy(g_tokenString, "comment error");

    if (g_traceScan) {
        fprintf(g_dest, "%-10d", g_line);
        printToken(currentToken, g_tokenString);
    }

    return currentToken;
}
