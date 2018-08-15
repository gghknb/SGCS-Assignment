/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

#define VAR   1  //variable
#define ARR   2  //array
#define PAR   4  //parameter
#define PARA  8  //parameter array
#define FUN   16 //function

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec {
	int lineno;
	struct LineListRec * next;

} *LineList; /* EDIT : POINTER TPYE FOR LINKED LIST LINELIST */

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */

typedef struct BucketListRec {
	/* ELEMENTS OF THIS STRUCTURE ARE
	 * location : memory offset
	 * name : name for symbol
	 * line numbers : line number that this symbol was appeared with linked-list
	 * Var  : if this symbol was variable then TRUE else FALSE (1)
	 * Para : if this symbol was parameter then TRUE else FLASE (2)
	 * Func : if this symbol was function then TURE else FLASE (4)
	 * Arr  : if this symbol was array then TRUE else FALSE (8)
	 * these 4 type was managed by flag bit masking
	 * type   : int void
	 * scope  : scope that the number of compound statements wrapped on this symbol
	 * length : if array? length = size of array
	 */
	char * name;
	LineList lines;
	ExpType type;
	int scope;
	int size;
    int location;
    int flag;
	struct BucketListRec * next;
	TreeNode *tree; // edit compare call argument decl parameter
} *BucketList;

void scopeUp(int);
void scopeDown();

//function for checking the flag : return nonzero -> TRUE, 0 -> FLASE
int isVar(BucketList a);
int isArr(BucketList a);
int isPar(BucketList a);
int isFun(BucketList a);
int isPara(BucketList a);

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(TreeNode *, int);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList st_lookup ( char * name );

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
