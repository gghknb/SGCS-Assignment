/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for C- Language                */
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
"int"			{return INT;}
"return"		{return RETURN;}
"void"			{return VOID;}
"while"			{return WHILE;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"<"             {return LT;}
"<="			{return LTE;}
">"				{return GT;}
">="			{return GTE;}
"=="			{return EQ;}
"!="			{return NEQ;}
"="             {return ASSIGN;}
";"             {return SEMI;}
","				{return COMMA;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"{"             {return LBRACE;}
"}"				{return RBRACE;}
"["             {return LBRACK;}
"]"             {return RBRACK;}
{number}        {return NUM;}
{identifier}    { 
					yylval = newExpNode(IdK);
					yylval->attr.name = strdup(yytext);
					yylval->lineno = lineno;
					return ID;
				}
{whitespace}    {/* skip whitespace */}
{newline}       {lineno++;}
"/*"            { char c;
                  int flag=0;

                  while(1){
                      c=input();
                      if(c=='*'){
                          c=input();
                          if(c=='/'){
                              flag=1; break;
                          }
                      }
                      if(c==EOF)
                          return COMERROR;
                      if(c=='\n')
                          lineno++;
                  }
                  if(flag!=1)
                      return COMERROR;
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
    fprintf(listing,"\t%d\t\t",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

yywrap()
{}
