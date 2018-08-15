/****************************************************/
/* File: util.h                                     */
/* Utility functions for the C- Language compiler   */
/* Compiler Contruction: Principles and Practice    */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType tok, const char *lex);

char* allocString(char *src);
void appendIOFunc(const char *fname, const char *fnameTmp);

/* Function newDeclInfo creates a new statement
 * node for syntax tree construction
 */
declInfo* newDeclInfo(char *id, int loc, int accum_loc, VPF vpf, auxInfo aux);
void appendLine(declInfo *decl, int line);
void appendParam(auxInfo *aux, int isArr);

/* for Hash function */
int hashFunc(char *str);
void insertHash(scopeHash *current, declInfo *decl);
declInfo* searchHash(scopeHash *current, int mode, char *id);
scopeHash* searchScopeByFunc(scopeHash *current, char *name);
scopeHash* searchParentFuncScope(scopeHash *current);

/* for Symbol Table */
scopeHash* newScopeHash(int fp, int scope, char *name);
void printScopeHash(scopeHash *global);
void destroyScopeHash(scopeHash *global);
void appendChildScopeHash(scopeHash *current, scopeHash *child);
scopeHash* getUnvisitedChildScopeHash(scopeHash *current);

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(AST_node *root);
void printSpace(int level);
void destroyTree(AST_node *root);
AST_node* newASTNode(NodeKind kind, SubKind subkind);

#endif
