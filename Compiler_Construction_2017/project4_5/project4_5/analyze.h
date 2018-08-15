/****************************************************/
/* File: analyze.h                                  */
/* Semantic analyzer interface for TINY compiler    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _ANALYZE_H_
#define _ALANYZE_H_

#include "globals.h"

/* flag */
static int accum_loc = -4;
static int funcNumber = 0;     
static int funcBodyFlag = 0; 
static int bsflag = 1;
static int nnflag = 0;
static int mainflag = 1;
static int buildflag = 1;
static int stopflag =1;
static int type2flag = 1;

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymTab(scopeHash *current, AST_node *root);

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(scopeHash *current, AST_node *root);
void mainCheck(scopeHash *current);

void semanticError(int line, const char *message);
void start(scopeHash* current);
void find(scopeHash* save, scopeHash* current);

#endif
