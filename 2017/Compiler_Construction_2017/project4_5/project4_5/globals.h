/****************************************************/
/* File: globals.h                                  */
/* Yacc/Bison Version                               */
/* Global types and vars for C- Language compiler   */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Yacc/Bison generates internally its own values
 * for the tokens. Other files can access these values
 * by including the tab.h file generated using the
 * Yacc/Bison option -d ("generate header")
 *
 * The YYPARSESR flag prevents inclusion of the tab.h
 * into the Yacc/Bison output itself
 */

#ifndef YYPARSER

/*ENDFILE is implicitly defined by Yacc/Bison.
 * and not included in the tab.h file
 */
#include "y.tab.h"

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXKEYWORDS 6   // the number of keywords.
#define MAXCHILDREN 3
#define HASH_SIZE 20

extern int g_line;
extern FILE *g_src;
extern FILE *g_dest;

extern int g_error;
extern int g_echoSrc;
extern int g_traceScan;
extern int g_traceCode;
extern int g_traceParse;

typedef int TokenType;
typedef union{int val;char* name;} vn;

typedef enum {
    VARDECL, FUNDECL,
    RETSTMT, SELSTMT, ITERSTMT, COMPSTMT,
    VAR, PARAM, PARAMS,
    CALL, EXPR, FACTOR, RELOP, ADDOP, MULOP,
    TYPESPEC
} NodeKind;
typedef enum {NONE, SOL, ARR} SubKind;
typedef enum {V, P, F} VPF; //Variable, Parameter, Function

typedef struct _Attr {
    char *name;
    int length;
    int line;
    int par;
    int val;
    TokenType what;
} Attr;

// AST tree node structure.
typedef struct _AST_node {
    Attr attr;
    NodeKind kind;
    SubKind subkind;
    struct _AST_node *sibling;
    struct _AST_node *child[MAXCHILDREN];
	struct _scopeHash *scope;
} AST_node;

typedef struct _listNode {
    int isArr;
    int line;     
    struct _listNode *link;
} listNode;

typedef union _auxInfo {
    struct { int isArr; int arrSize; } vpInfo;   
    struct { int retType; listNode *params;} fInfo;   
} auxInfo;

typedef struct _declInfo {
    char *id;
	int	accum_loc;
    int loc;
    auxInfo aux;
    VPF vpf;
    listNode *lines;
    struct _declInfo *link;
} declInfo;

typedef struct _scopeHash {
    char *name;
    int fp;
    int scope;
    int visit;
	int min;
	int max;
    declInfo *decls[HASH_SIZE];
    struct _scopeHash *parent;
    struct _scopeHash *sibling;
    struct _scopeHash *children;
} scopeHash;
#endif
