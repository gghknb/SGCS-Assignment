/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
">"             {return MT;}
">="            {return MTEQ;}
"<"             {return LT;}
"<="            {return LTEQ;}
"=="            {return EQ;}
"!="            {return NEQ;}
"="             {return ASSIGN;}
";"             {return SEMI;}
","             {return CM;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LSB;}
"]"             {return RSB;}
"{"             {return LCB;}
"}"             {return RCB;}

{number}        {savedName = copyString(yytext); return NUM;}
{identifier}    {savedName = copyString(yytext); return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"             { char c,prev_c;
                   int end = 0;
                  while(1)
                  { c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                    if (c == '/' && prev_c == '*')
                       end = 1;
                    prev_c = c;
                    if(end ==  1)
                        break;
                  } 
                  if(end == 0) return COMMENTS;
                }
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d\t",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

