/****************************************************/
/* File: tiny.y                                     */
/* The C- Language Yacc/Bison specification file    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */
#define YYSTYPE AST_node*

#include <stdlib.h>
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static int savedNUM;
static char *savedID;
static AST_node *root; /* stores syntax tree for later retrun */
static int yylex(void);

%}

%token ENDFILE ERROR
%token IF ELSE INT VOID WHILE RETURN
%token ID NUM
%token ASSIGN SEMI COMMA
%token PLUS MINUS MUL DIV
%token LT LET GT GET EQ NEQ
%token LPRTS RPRTS LBRKT RBRKT LBRC RBRC LCMT RCMT
%nonassoc IFX
%nonassoc ELSE

%% /* Grammar for TINY */

program             :   declaration_list    
                        { root = $1; }
                    ;

declaration_list    :   declaration_list declaration
                        { YYSTYPE t = $1;
                          if (t!=NULL){
                            while(t->sibling)
                                t = t->sibling;
                            t->sibling = $2;
                            $$ = $1;
                            }
                            else $$ = $2;
                        }
                    |   declaration { $$ = $1; }
                    ;

declaration         :   var_declaration { $$ = $1; }
                    |   fun_declaration { $$ = $1; }
                    ;

var_declaration     :   type_specifier read_identifier SEMI
                        {
                            $$ = newASTNode(VARDECL, SOL);
                            $$->attr.name = savedID;
                            $$->child[0] = $1;
                        }
                    |   type_specifier read_identifier LBRKT read_number RBRKT SEMI
                        {
                            $$ = newASTNode(VARDECL, ARR);
                            $$->attr.name = savedID;
                            $$->child[0] = $1;
                            $$->attr.length = savedNUM;
                        }
                    ;

type_specifier      :   INT{$$ = newASTNode(TYPESPEC, NONE);
                            $$->attr.what = INT;
                        }
                    |   VOID{$$ = newASTNode(TYPESPEC, NONE);
                             $$->attr.what = VOID;
                        }
                    ;

fun_declaration     :   type_specifier read_identifier{
                            $$ = newASTNode(FUNDECL, NONE);
                            $$->attr.name = savedID;
                        }LPRTS params RPRTS compound_stmt{
                            $$ = $3;
                            $$->child[0] = $1;
                            $$->child[1] = $5;
                            $$->child[2] = $7;
                        }
                    ;

params              :   param_list{ $$ = $1; }
                    |   VOID{
                            $$ = newASTNode(PARAMS, NONE);
                            $$->attr.what = VOID;
                        }
                    ;

param_list          :   param_list COMMA param
                        {
                            YYSTYPE t = $1;
                            if (t != NULL){
                                while(t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $3;
                                $$ = $1;
                            }
                            else $$ = $3;
                        }
                    |   param { $$ = $1; }
                    ;

param               :   type_specifier read_identifier
                        {
                            $$ = newASTNode(PARAM, SOL);
                            $$->child[0] = $1;
                            $$->attr.name = savedID;
                        }
                    |   type_specifier read_identifier LBRKT RBRKT
                        {
                            $$ = newASTNode(PARAM, ARR);
                            $$->child[0] = $1;
                            $$->attr.name = savedID;
                        }
                    ;

compound_stmt       :   LBRC local_declarations statement_list RBRC
                        {
                            $$ = newASTNode(COMPSTMT, NONE);
                            $$->child[0] = $2;
                            $$->child[1] = $3;
                        }
                    ;

local_declarations  :   local_declarations var_declaration
                        {
                            YYSTYPE t = $1;
                            if (t != NULL){
                                while (t->sibling)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            }
                            else $$ = $2;
                            }
                    |   /* empty */ { $$ = NULL; }
                    ;

statement_list      :   statement_list statement
                        {
                            YYSTYPE t = $1;
                            if (t!=NULL){
                                while (t->sibling)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            }
                            else $$ = $2;
                        }
                    |   /* empty */ { $$ = NULL; }
                    ;

statement           :   expression_stmt { $$ = $1; }
                    |   compound_stmt   { $$ = $1; }
                    |   selection_stmt  { $$ = $1; }
                    |   iteration_stmt  { $$ = $1; }
                    |   return_stmt     { $$ = $1; }
                    ;

expression_stmt     :   expression SEMI { $$ = $1; }
                    |   SEMI            { $$ = NULL; }
                    ;

selection_stmt      :   IF LPRTS expression RPRTS statement %prec IFX
                        {
                            $$ = newASTNode(SELSTMT, NONE);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                        }
                    |   IF LPRTS expression RPRTS statement ELSE statement
                        {
                            $$ = newASTNode(SELSTMT, NONE);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                            $$->child[2] = $7;
                        }
                    ;

iteration_stmt      :   WHILE LPRTS expression RPRTS statement
                        {
                            $$ = newASTNode(ITERSTMT, NONE);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                        }
                    ;

return_stmt         :   RETURN SEMI{$$ = newASTNode(RETSTMT, NONE);}
                    |   RETURN expression SEMI
                        {
                            $$ = newASTNode(RETSTMT, NONE);
                            $$->child[0] = $2;
                        }
                    ;

expression          :   var ASSIGN expression
                        {
                            $$ = newASTNode(EXPR, NONE);
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   simple_expression { $$ = $1; }
                    ;

var                 :   read_identifier
                        {
                            $$ = newASTNode(VAR, SOL);
                            $$->attr.name = savedID;
                        }
                    |   read_identifier{
                            $$ = newASTNode(VAR, ARR);
                            $$->attr.name = savedID;
                        }LBRKT expression RBRKT{
                            $$ = $2;
                            $$->child[0] = $4;
                        }
                    ;

simple_expression   :   additive_expression LT additive_expression
                        {
                            $$ = newASTNode(RELOP, NONE);
                            $$->attr.what = LT;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression LET additive_expression
                        {
                            $$ = newASTNode(RELOP, NONE);
                            $$->attr.what = LET;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression GT additive_expression
                        {
                            $$ = newASTNode(RELOP, NONE);
                            $$->attr.what = GT;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression GET additive_expression
                        {
                            $$ = newASTNode(RELOP, NONE);
                            $$->attr.what = GET;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression EQ additive_expression
                        {
                                $$ = newASTNode(RELOP, NONE);
                                $$->attr.what = EQ;
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                            }
                    |   additive_expression NEQ additive_expression
                        {
                            $$ = newASTNode(RELOP, NONE);
                            $$->attr.what = NEQ;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression { $$ = $1; }
                    ;

additive_expression :   additive_expression PLUS term
                        {
                            $$ = newASTNode(ADDOP, NONE);
                            $$->attr.what = PLUS;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   additive_expression MINUS term
                        {
                            $$ = newASTNode(ADDOP, NONE);
                            $$->attr.what = MINUS;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   term { $$ = $1; }
                    ;

term                :   term MUL factor
                        {
                            $$ = newASTNode(MULOP, NONE);
                            $$->attr.what = MUL;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                         }
                    |   term DIV factor
                        {
                            $$ = newASTNode(MULOP, NONE);
                            $$->attr.what = DIV;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                        }
                    |   factor  { $$ = $1; }
                    ;

factor              :   LPRTS expression RPRTS  { $$ = $2; }
                    |   var                     { $$ = $1; }
                    |   call                    { $$ = $1; }
                    |   read_number
                        {
                            $$ = newASTNode(FACTOR, NONE);
                            $$->attr.val = savedNUM;
                        }
                    ;

call                :   read_identifier
                        {
                            $$ = newASTNode(CALL, NONE);
                            $$->attr.name = savedID;
                        }
                        LPRTS args RPRTS
                        {
                            $$ = $2;
                            $$->child[0] = $4;
                        }
                    ;

args                :   arg_list { $$ = $1; }
                    |   /* empty */ { $$ = NULL; }
                    ;

arg_list            :   arg_list COMMA expression
                        {
                            YYSTYPE t = $1;
                            if (t!=NULL){
                                while(t->sibling)
                                    t = t->sibling;
                                t->sibling = $3;
                                t->sibling->attr.par = TRUE;
                                $$ = $1;
                            }
                            else $$ = $3;
							$$->attr.par = TRUE;
                        }
                    |   expression
                        {
                            $$ = $1;
                            $$->attr.par = TRUE;
                        }
                    ;

read_number         :   NUM{ savedNUM  = atoi(g_tokenString);}
                    ;

read_identifier     :   ID{ savedID = allocString(g_tokenString);}
                    ;
%%

static int yylex(void) {
    return getToken();
}

int yyerror(char *message) {
    fprintf(g_dest, "[syntax error] line %3d : %s\n", g_line, message);
    fprintf(g_dest, "       [token] %s\n", g_tokenString);
    g_error = TRUE;
    return 0;
}

AST_node *parse(void) {
    yyparse();
    return root;
}
