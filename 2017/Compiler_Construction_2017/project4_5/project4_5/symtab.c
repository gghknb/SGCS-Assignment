/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

static int st_scope=-1; //scope

/* the hash table EDIT : POINTER TYPE */
// static BucketList hashTable[SIZE];
typedef struct SymbolTableRec {
	BucketList hashTable[SIZE];
	int location;
	int fp;
	struct SymbolTableRec* next;

} *SymbolTable;

SymbolTable symbolTable;

/* the hash function */
static int hash ( char * key ) { 
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{ 
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(TreeNode *a, int flag) {
    char *name;
    int lineno;

    if(flag==0){
        name=a->child[1]->attr.name;
        lineno=a->child[1]->lineno;
    }
    else{
        name=a->attr.name;
        lineno=a->lineno;
    }
    int h=hash(name);
	
	if (flag){
		BucketList bl = st_lookup(name);
		if (bl == NULL)
			fprintf(listing, "Error in line %d : variable %s not exist\n",a->lineno, name);
		else {
			LineList l = bl->lines;
			if (l == NULL){
				l = (LineList) malloc(sizeof(struct LineListRec));
				l->lineno = lineno;
				l->next = NULL;
				bl->lines = l;
			} else {
				if (l->lineno == lineno) return;
				while (l->next != NULL) {
					if (l->next->lineno == lineno) return;
					l = l->next;
				}

				l->next = (LineList)malloc(sizeof(struct LineListRec));
				l->next->lineno = lineno;
				l->next->next = NULL;
			}
		}
		return;
	}

	BucketList l =  symbolTable->hashTable[h];
	while ((l != NULL) && (strcmp(name,l->name) != 0))
		l = l->next;
	
	if (l == NULL) { /* variable not yet in table */ 
		l = (BucketList) malloc(sizeof(struct BucketListRec));
		/* initialize */
        l->name = name;
		l->location = (symbolTable->location)++;
		l->lines = NULL;
		l->size=0;
        l->flag=0;
        if(a->kind.dec==VarK)
            l->flag|=VAR;
      //  if(a->kind.dec == FunK)
      //      l->flag |= FUN;
        else if (a->kind.dec == ArrK) {
			l->size = a->child[1]->range;
            l->flag |= ARR;
            if(a->scope == Param)
               l->flag |= PARA;
        }
        if(a->kind.dec == FunK) 
            l->flag |= FUN;
        if (a->scope == Param) 
            l->flag |= PAR; 

		l->type = a->type;
		l->scope = st_scope; 
		l->next = symbolTable->hashTable[h];
        l->tree = a;
		symbolTable->hashTable[h] = l;
	} 
    else 
		fprintf(listing,"Error in line %d : %s multiple declaration\n", a->lineno, name);

	if((a->kind.dec == VarK || a->scope == Param || a->kind.dec == ArrK) && a->kind.dec != FunK){
		if(a->type == Void)
			fprintf(listing,"Error in line %d : Variable and Parameter have VOID type\n", a->lineno); 
	}

} /* st_insert */

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList st_lookup ( char * name ) {
	int h = hash(name);
	SymbolTable temp = symbolTable;

	while (temp != NULL){
		BucketList l =  temp->hashTable[h];
		while ((l != NULL) && (strcmp(name,l->name) != 0))
			l = l->next;
		if (l != NULL) return l;
		temp = temp->next;
	}
	
	return NULL;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing) {

	int i;
	fprintf(listing,"Variable Name  Scope   Location  V/P/F  Array?  ArrSize  Type  Line Numbers\n");
	fprintf(listing,"-----------------------------------------------------------------------------------------------------\n");
	for (i = 0; i < SIZE; i++) {
		if (symbolTable->hashTable[i] != NULL) {
			BucketList l = symbolTable->hashTable[i];
			while (l != NULL) {
				
				LineList t = l->lines;
				
				fprintf(listing, "%-13s  ", l->name);
				fprintf(listing, "%-5d  " , l->scope);
				fprintf(listing, "%-9d  " , l->location);
				
				if(isFun(l))
                        fprintf(listing, "%-6s","Func");
                else if(isPar(l))
                    fprintf(listing, "%-6s", "Par");
                else
                    fprintf(listing, "%-6s", "Var");
				
				fprintf(listing, "%-6s  ", isArr(l)? "Array":"No");
				
				if (isPara(l))
                    fprintf(listing, "%-7s  ", "param");
                else if(isArr(l))
                    fprintf(listing, "%-7d  ", l->size);
                else
                    fprintf(listing, "%-7s  ", "-"); 
				
				fprintf(listing, "%-5s  ", l->type==Void? "void":"int");
				
				while (t != NULL) { 
					fprintf(listing,"%4d ",t->lineno);
					t = t->next;
				}
				fprintf(listing,"\n");
				l = l->next;
			}
		}
	}
	fprintf(listing, "\n");

} /* printSymTab */

void free_BucketList(SymbolTable a) {
	int i;
	BucketList temp, del;
	LineList line, dele;

	for (i = 0; i < SIZE; i++) {
		temp = a->hashTable[i];
		while (temp != NULL) {
			del = temp;
			temp = temp->next;
			line = del->lines;

			while (line != NULL) {
				dele = line;
				line = line->next;
				free(dele);
			}
			free(del);
		}
	}
}

void scopeUp(int flag) {
	int i;
	SymbolTable new = malloc(sizeof(struct SymbolTableRec));

	new->next = symbolTable;
	new->location = 0;
	new->fp = 0;

	if (flag) {
		new->location = symbolTable->location;
		new->fp = symbolTable->fp;
	}
	symbolTable = new;

	for (i = 0; i < SIZE; i++)
		symbolTable->hashTable[i] = NULL;
	st_scope++;
}

void scopeDown() {
	SymbolTable del;

	if (st_scope<0) { //scope이 초기값인 -1로 설정되어 있는 경우
		printf("error in scope down");
		exit(0);
	}

	if (TraceAnalyze)
		printSymTab(listing);

	st_scope--;
	del = symbolTable;
	symbolTable = symbolTable->next;

	free_BucketList(del);
	free(del);
}

void Set_Pointer() {
	if (symbolTable->fp != 0) {
		scopeUp(1);
		symbolTable->location = 0;
		return;
	}

	symbolTable->fp = (symbolTable->location) * 4 + 4;
}

int isVar(BucketList a) { return a->flag & VAR; }
int isArr(BucketList a) { return a->flag & ARR; }
int isPar(BucketList a) { return a->flag & PAR; }
int isFun(BucketList a) { return a->flag & FUN; }
int isPara(BucketList a){ return a->flag & PARA;}
