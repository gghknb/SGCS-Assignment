/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/


#ifndef YYPARSER
#include "cm.tab.h"
#define ENDFILE 0
#endif

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8
typedef int TokenType;
// for project 1

/* typedef enum 
     book-keeping tokens 
   {
    ENDFILE,ERROR,COMMENTS, 
    reserved words 
    IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,
     multicharacter tokens 
    ID,NUM,INT,RETURN,VOID,WHILE,
     special symbols 
    PLUS,MINUS,TIMES,OVER,MT,MTEQ,LT,LTEQ,EQ,NEQ,ASSIGN,SEMI,CM,LPAREN,RPAREN,LSB,RSB,LCB,RCB// + - * / > >= < <= == != = ; , ( ) [ ] ( )  
   } TokenType;
*/
extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK,ExpK,DeclK,TypeK,ParaK,CallK} NodeKind;
typedef enum {IfK,ReturnK,WhileK,IfelK,CompK,ReturnexpK} StmtKind;
typedef enum {OpK,ConstK,IdK,ArrK,SimpleK,EpK} ExpKind;
typedef enum {VarK,VarrK,FuncK} DeclKind;
typedef enum {IntK,VoidK} TypeKind;
typedef enum {PidK,ParK} ParaKind;
typedef enum {CalllK} CallKind;
/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;DeclKind decl; TypeKind type; ParaKind para; CallKind call;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     ExpType type; /* for type checking of exps */
   } TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int array_flag;
extern int assign_flag;
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern char *savedName;
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
