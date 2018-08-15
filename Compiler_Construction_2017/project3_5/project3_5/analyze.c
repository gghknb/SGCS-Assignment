/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

int t_flag = 0;
int ty_flag = 0;
int main_flag = 0;
static void checkNode(TreeNode *t);
/* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse(TreeNode * t,
	void(*preProc) (TreeNode *),
	void(*postProc) (TreeNode *))
{
	if (t != NULL)
	{
		preProc(t);
		{ int i;
		for (i = 0; i < MAXCHILDREN; i++)
			traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t){
	if (t==NULL) return;
	else return;
}

static void closeProc(TreeNode *t){
	if (t->nodekind == StmtK && t->kind.stmt == CompK)
		scopeDown();
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t) {
	//test for symbol table functions
	switch (t->nodekind){
		case StmtK:
			if(t->kind.stmt==CompK) {
				Set_Pointer();break;
			} 
			break;
		case DecK:
			st_insert(t, 0);
			if(t->kind.dec==FunK){	
				scopeUp(0); break;
			}
			break;
		case ExpK:
			switch(t->kind.exp){
				case IdK:
				case CallK:
					st_insert(t, 1);
					break;
			}
			break;
		default: break;
	}
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree) { 
	fprintf(listing, "symbol table functions test\n");
	scopeUp(0);

	if (TraceAnalyze)
		fprintf(listing,"\nSymbol table:\n\n");
	
	traverse(syntaxTree,insertNode,checkNode);
	scopeDown();
}

static void typeError(TreeNode * t, char * message) { 
	fprintf(listing,"Error in line %d : %s\n", t->lineno, message);
	Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */

/* 0 is integer    1 is array     
 * 2 is ERROR      3 is void */
static void checkNode(TreeNode * t) {	
	int meet_flag = 0;
	//type check - checking for symbol table type check
	switch (t->nodekind) {
		case DecK:
			if (t->kind.dec == FunK) {
				if (t->type == Void) {
					if (t_flag * t_flag == 4)
						typeError(t->child[1], "Error 'Return not required'"); //함수의 type이 void인데 return이 존재 단 return;의 경우에는 오류로 체크하지 않는다.
				}
				else {
					if (t_flag != 2)
						typeError(t->child[1], "Error 'Return is necessary'"); //함수의 type이 integer형이지만 return이 존재하지 않는 경우
				}
				t_flag = 0;

				if (strcmp((t->child[1])->attr.name, "main") == 0) {
					main_flag = 1;
					if (t->child[2] != NULL)typeError(t->child[1], "Error 'main function parameter must be Void type'"); //메인 함수에서 parameter가 void형이 아닌 것이 존재할 경우
					if (t->type != Void)
                        typeError(t->child[1],"Error 'Main function type is only void'");
                }
                else {
                    if(main_flag ==1)
                         typeError(t->child[1], "Error 'main must be declared at the end'");
                }
			}
			break;
		case ExpK:
			if (t->kind.exp == ConstK)
				t->final_type = 0;

			if (t->kind.exp == IdK) {
				BucketList l = st_lookup(t->attr.name);
				if (l == NULL)
					t->final_type = 2;
				else if (isArr(l) != FALSE) {
					if (t->child[0] == NULL)
						t->final_type = 1;
					else if ((t->child[0])->final_type == 0)
						t->final_type = 0;
					else {
						typeError(t, "Error 'array index must be number'"); //배열 index에 숫자가 아닌 다른 type이 들어 갈 경우
						t->final_type = 2;
					}

					if (l->type == Void)
						t->final_type = 2;
				}
				else if (isFun(l) != FALSE)
					t->final_type = 2;
				else {
					t->final_type = 0;
					if (l->type == Void)
						t->final_type = 2;
					else if (t->child[0] != NULL) {
						typeError(t, "Error 'Not using variable like array'"); //배열이 아닌 변수를 배열처럼 사용할 경우
						t->final_type = 2;
					}
				}
			}
			else if (t->kind.exp == OpK || t->kind.exp == AssignK) {
				TreeNode *rchild = t->child[0];
				TreeNode *lchild = t->child[1];
				if (rchild->final_type == lchild->final_type && rchild->final_type == 0)
					t->final_type = 0;
				else {
					t->final_type = 2;
					typeError(t, "Error in Op / Assign"); //assign이나 operator일 경우 양 쪽의 type이 맞지 않는 경우
				}
			}
			else if (t->kind.exp == CallK) {
				BucketList bl = st_lookup(t->attr.name);
				if (bl == NULL) {
					t->final_type = 2;
				}
				else if (isFun(bl) != FALSE) {
					if (bl->type == Void) t->final_type = 3;
					else t->final_type = 0;

					if (t->child[0] != NULL) {
						TreeNode *arg_t = (t->child[0])->child[0];;
						TreeNode *par_t = (bl->tree)->child[2];
			
						while (arg_t != NULL && par_t != NULL) {
							if (par_t->kind.dec == VarK && arg_t->final_type == 0)meet_flag = 1;
							else if (par_t->kind.dec == ArrK && arg_t->final_type == 1)meet_flag = 1;
							if (meet_flag == 1) {
								par_t = par_t->sibling;
								arg_t = arg_t->sibling;
								meet_flag = 0;
							}
							else break;
						}

						if (arg_t != NULL || par_t != NULL) {
							t->final_type = 2;
					 		typeError(t, "Error 'call function argument do not match with decl function parameter'"); //함수 호출 시 함수의 parameter와 함수 호출 argument 개수가 맞지 않는 경우
						}
						else
							t->final_type = 0;
					}
				}
				else {
					t->final_type = 2;
					typeError(t, "Error 'It is not function call'"); //함수 호출 시 호출 된 것이 함수가 아닐 경우
				}
			}
			break;
		default: break;
	}

	if(t->nodekind == StmtK && t -> kind.stmt == IterK){
		if((t -> child[0]) -> final_type != 0)
			typeError(t->child[0], "Error 'Loop statement can be expression or integer'");
	}
	if (t->nodekind == StmtK && t->kind.stmt == SelecK) {
		if ((t->child[0])->final_type != 0)
			typeError(t->child[0], "Error 'if statement can be expression or integer'");
	}
	if(t -> nodekind == StmtK && t -> kind.stmt == RetK){
		if(t -> child[0] != NULL){
			if((t->child[0])->final_type == 0)t_flag = 2;
			else t_flag = -2;
		}
		else t_flag = 1;
	}
	if (t->nodekind == StmtK && t->kind.stmt == CompK)
		scopeDown();
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree) {
//	traverse(syntaxTree,nullProc,checkNode);
}
