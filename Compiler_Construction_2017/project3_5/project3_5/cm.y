/****************************************************/
/* File: tiny.y                                     */
/* The C- Language Yacc/Bison specification file    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static int savedLineNo;  /* ditto */
static int savedNumber;	 /* for use in array */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex
extern char *yytext;

%}

%token ELSE IF INT RETURN VOID WHILE
%token ID NUM 
%token PLUS MINUS TIMES OVER LT LTE GT GTE EQ NEQ ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token ERROR COMERROR

%nonassoc LT LTE GT GTE EQ NEQ
%left PLUS MINUS 
%left TIMES OVER

%% /* Grammar for TINY */

program		: decl_list
				{ savedTree = $1; }
			;

decl_list	: decl_list decl
				{ YYSTYPE t = $1;
				  if (t != NULL) {
					while (t->sibling != NULL)
					    t = t->sibling;
					t->sibling = $2;
					$$ = $1;
				  }
                  else $$ = $1;
				}
			| decl { $$ = $1; }
			;

decl		: var_decl{ $$ = $1; $$->scope = Global; }
			| fun_decl{ $$ = $1; $$->scope = Global; }
			;

var_decl	: type_specifier ID SEMI{ 
							$$ = newDecNode(VarK);
							$$->child[0] = $1;
							$$->type = $1->kind.type;
							$2->range = 0;
							$$->child[1] = $2;
						}
			| type_specifier ID LBRACK NUM { savedNumber = atoi(tokenString); } RBRACK SEMI{
							$$ = newDecNode(ArrK);
							$$->child[0] = $1;
							$$->type = $1->kind.type;
							$2->range = savedNumber;
							$$->child[1] = $2;
			            }
			;

type_specifier		: INT{ 
							$$ = newTypeNode(IntK);
							$$->attr.name = copyString(tokenString);
						}
					| VOID{
							$$ = newTypeNode(VoidK);
							$$->attr.name = copyString(tokenString);
						}
					;

fun_decl	: type_specifier ID LPAREN params RPAREN compound_stmt{
							$$ = newDecNode(FunK);
							$$->child[0] = $1;
							$$->type = $1->kind.type;
							$$->child[1] = $2; 
							$$->child[2] = $4;
							$$->child[3] = $6;
						}
			;

params		: param_list{ $$ = $1; }
			| VOID{ $$ = NULL; }
			;

param_list	: param_list COMMA param{
							YYSTYPE t = $1;
							if (t != NULL) {
								while (t->sibling != NULL)
									t = t->sibling;
								t->sibling = $3;
								$$ = $1;
                               
							}else {
								$$ = $3;
							}
						}
			| param{ $$ = $1; 
                 }
			;

param		: type_specifier ID	{
							$$ = newDecNode(VarK);
							$$->scope = Param;
							$$->child[0] = $1;
							$$->type = $1->kind.type;
							$$->child[1] = $2;
						}
			| type_specifier ID LBRACK RBRACK{
							$$ = newDecNode(ArrK);
							$$->scope = Param;
							$$->child[0] = $1;
							$$->range = 0;
							$$->type = $1->kind.type;
							$$->child[1] = $2;
						}
			| error{ $$ = newExpNode(ErrK);}
			;

compound_stmt		: LBRACE local_decl statement_list RBRACE{
							$$ = newStmtNode(CompK);
							$$->child[0] = $2;
							$$->child[1] = $3;
						}
					;

local_decl	: local_decl var_decl{
					YYSTYPE t = $1;
					$2->scope = Local;
					if (t != NULL){
						while (t->sibling != NULL)
							t = t->sibling;
						t->sibling = $2;
						$$ = $1;
					}else $$ = $2;
				}
			| /* empty */{ $$ = NULL; }
			;

statement_list		: statement_list statement{
							YYSTYPE t = $1;
							if (t != NULL){
								while (t->sibling != NULL)
									t = t->sibling;
								t->sibling = $2;
								$$ = $1;
							}else {
								$$ = $2;
							}
						}
					| /* empty */{ $$ = NULL; }
					;

statement			: expression_stmt{ $$ = $1; }
					| compound_stmt  { $$ = $1; }
					| selection_stmt { $$ = $1; }
					| iteration_stmt { $$ = $1; }
					| return_stmt    { $$ = $1; }
					;

expression_stmt		: expression SEMI{ 
							$$ = newStmtNode(ExprK);
							$$->child[0] = $1;
						}
					| SEMI{ $$ = NULL; }
					;

selection_stmt		: IF LPAREN expression RPAREN statement{ 
							$$ = newStmtNode(SelecK);
							$$->child[0] = $3;
							$$->child[1] = $5;
						}
					| IF LPAREN expression RPAREN statement ELSE statement{
							$$ = newStmtNode(SelecK);
							$$->child[0] = $3;
							$$->child[1] = $5;
							$$->child[2] = $7;
						}
					;

iteration_stmt		: WHILE LPAREN expression RPAREN statement{
							$$ = newStmtNode(IterK);
							$$->child[0] = $3;
							$$->child[1] = $5;
						}
					;

return_stmt			: RETURN SEMI {$$ = newStmtNode(RetK);}
					| RETURN expression SEMI{
							$$ = newStmtNode(RetK);
							$$->child[0] = $2;
						}
					;

expression			: var ASSIGN expression{
							$$ = newExpNode(AssignK);
							$$->attr.name = $1->attr.name;
							$$->child[0] = $1;
							$$->child[1] = $3;
						}
					| simple_expression{ $$ = $1;}
					;

var					: ID{ $$ = $1; }
					| ID LBRACK expression RBRACK{
							$$ = $1;
							$$->child[0] = $3;
						}
					;

simple_expression	: additive_expression { $$ = $1; }
					| additive_expression relop additive_expression{
							$$ = $2;
							$$->child[0] = $1;
							$$->child[1] = $3;
						}
					;

relop				: LTE{
							$$ = newExpNode(OpK);
							$$->attr.op = LTE;
						}
					| LT{
							$$ = newExpNode(OpK);
							$$->attr.op = LT;
						}
					| GT{
							$$ = newExpNode(OpK);
							$$->attr.op = GT;
						}
					| GTE{
							$$ = newExpNode(OpK);
							$$->attr.op = GTE;
						}
					| EQ{
							$$ = newExpNode(OpK);
							$$->attr.op = EQ;
						}
					| NEQ{
							$$ = newExpNode(OpK);
							$$->attr.op = NEQ;
						}
					;

additive_expression	: additive_expression addop term{
							$$ = $2;
							$$->child[0] = $1;
							$$->child[1] = $3;
						}
					| term { $$ = $1; }
					;

addop				: PLUS{
							$$ = newExpNode(OpK);
							$$->attr.op = PLUS;
						}
					| MINUS{
							$$ = newExpNode(OpK);
							$$->attr.op = MINUS;
						}
					;

term				: term mulop factor{
							$$ = $2;
							$$->child[0] = $1;
							$$->child[1] = $3;
						}
					| factor{ $$ = $1; }
					;

mulop				: TIMES{
							$$ = newExpNode(OpK);
							$$->attr.op = TIMES;
						}
					| OVER{
							$$ = newExpNode(OpK);
							$$->attr.op = OVER;
						}
					| error { $$ = newExpNode(ErrK); }
					;

factor				: LPAREN expression RPAREN { $$ = $2; }
					| var { $$ = $1; }
					| call{ $$ = $1; }
					| NUM{
							$$ = newExpNode(ConstK);
							$$->attr.val = atoi(tokenString);
						}
					| error{ $$ = newExpNode(ErrK); }
					;

call				: ID LPAREN args RPAREN{
							$$ = newExpNode(CallK);
							$$->attr.name = $1->attr.name;
							$$->child[0] = $3;
						}
					;

args				: arg_list{
							$$ = newExpNode(ArgK);
							$$->child[0] = $1;
						}
					| /* empty */{ $$ = NULL; }
					;

arg_list			: arg_list COMMA expression{
							YYSTYPE t = $1;
							if (t != NULL){
								while (t->sibling != NULL)
							 		t = t->sibling;
								t->sibling = $3;
								$$ = $1;
							}else {
								$$ = $3;
							}
						}
					| expression{ $$ = $1; }
					;

%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the C- Language scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

