/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static int savedNum;
static TokenType savedop;
char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex

%}

%token IF ELSE INT RETURN VOID WHILE  
%token LTEQ MT MTEQ EQ NEQ CM LSB RSB LCB RCB
%token ID NUM 
%token  ASSIGN LT PLUS MINUS TIMES OVER LPAREN RPAREN SEMI
%token ERROR COMMENTS

%nonassoc EQ LT LTEQ MT MTEQ ASSIGN NEQ
%left PLUS MINUS
%left TIMES OVER

%nonassoc IF
%nonassoc ELSE

%% /* Grammar for TINY */

program     : decl_list
                 { savedTree = $1;} 
            ;
decl_list   : decl_list decl
              { YYSTYPE t = $1;
                if(t != NULL)
                {  while(t->sibling != NULL)
                       t= t->sibling;
                   t->sibling = $2;
                   $$ = $1;
                }
                else $$ = $2;
              }
            |decl {$$ = $1; }
            ;
decl        : var_decl{ $$ = $1; }
            | func_decl{ $$ = $1;}
            ;

var_decl    : type_sp idsave SEMI { $$ = newDeclNode(VarK);
                      $$->child[0] = $1;
                      $$->child[1] = $2; }
            | type_sp idsave LSB numsave RSB SEMI { $$ = newDeclNode(VarrK);
                      $$->child[0] = $1;
                      $$->child[1] = $2;
                      $$->child[2] = $4;
                      $$->lineno = lineno;}
            ;
idsave      : ID { $$ = newExpNode(IdK);
                   $$->attr.name = savedName;}
            ;
numsave     : NUM { $$ = newExpNode(ConstK);
                    $$->attr.val = atoi(savedName);}
type_sp     : INT { $$ = newTypeNode(IntK);}
            | VOID { $$ = newTypeNode(VoidK);}
            ;
func_decl   : type_sp idsave LPAREN params RPAREN cmp_stmt { $$ = newDeclNode(FuncK);
                                        $$->child[0] = $1;
                                        $$->child[1] = $2;
                                        $$->child[2] = $4;
                                        $$->child[3] = $6;
                                        $$->lineno = lineno;}
            ;
params      : param_list {$$ = $1;}
            | VOID {$$ = newTypeNode(VoidK);}
            ;
param_list  : param_list CM param
             { YYSTYPE t = $1;
               if(t != NULL)
               {
                    while(t->sibling != NULL)
                        t = t->sibling;
                    t->sibling = $3;
                    $$ = $1;}
               else $$ = $3;}
            | param { $$ = $1;}
            ;
param       : type_sp idsave { $$ = newParaNode(PidK);
                         $$->child[0] = $1;
                         $$->child[1] = $2;
                         $$->lineno = lineno;}
            ;
            | type_sp idsave LSB RSB { $$ = newParaNode(ParK);
                            $$->child[0] = $1;
                            $$->child[1] = $2;
                            $$->lineno = lineno;}
            ;
cmp_stmt    : LCB loc_decl stmt_list RCB { $$ = newStmtNode(CompK);
                                           $$->child[0] = $2;
                                           $$->child[1] = $3;}
            ;
loc_decl    : loc_decl var_decl { YYSTYPE t = $1;
                                  if(t != NULL)
                                  { while(t->sibling != NULL)
                                        t = t->sibling;
                                    t->sibling = $2;
                                    $$ = $1;}
                                  else $$ = $2;}
            |{$$ = NULL;}
            ;
stmt_list   : stmt_list stmt {YYSTYPE t = $1;
                               if(t!= NULL)
                               { while(t->sibling != NULL)
                                    t = t->sibling;
                                 t->sibling = $2;
                                 $$ = $1;}
                               else $$ = $2;}
            |{$$ = NULL;}
            ;                             
/*
stmt_seq    : stmt_seq SEMI stmt
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $3;
                     $$ = $1; }
                     else $$ = $3;
                 }
            | stmt  { $$ = $1; }
            ;
*/
stmt        : exp_stmt { $$ = $1; }
            | cmp_stmt { $$ = $1; }
            | if_stmt { $$ = $1; }
            | while_stmt { $$ = $1; }
            | return_stmt { $$ = $1; }
            ;

exp_stmt    : exp SEMI { $$ = $1;}
            | SEMI {$$ = NULL;}
            ;

if_stmt     : IF LPAREN exp RPAREN stmt %prec IF{ $$ = newStmtNode(IfK);
                                      $$->child[0] = $3;
                                      $$->child[1] = $5;}
            | IF LPAREN exp RPAREN stmt ELSE stmt { $$ = newStmtNode(IfelK);
                                                    $$->child[0] = $3;
                                                    $$->child[1] = $5;
                                                    $$->child[2] = $7;}
            ;
while_stmt  : WHILE LPAREN exp RPAREN stmt { $$ = newStmtNode(WhileK);
                                             $$->child[0] = $3;
                                             $$->child[1] = $5;}
            ;
return_stmt : RETURN SEMI { $$ = newStmtNode(ReturnK);}
            | RETURN exp SEMI { $$ = newStmtNode(ReturnexpK);
                                $$->child[0] = $2;}
            ;
exp         : var ASSIGN exp {
                           $$ = newExpNode(EpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;}
            | simple_exp { $$ = $1;}
            ;

var         : idsave { $$ = $1;}
            | idsave LSB exp RSB { $$ = newExpNode(ArrK);
                                   $$->child[0] = $1;
                                   $$->child[1] = $3;
                                   $$->lineno = lineno;}

simple_exp  : add_exp relop { $$ = newExpNode(SimpleK);
                              $$->attr.op = savedop;}
              add_exp { $$ = $3;
                        $$->child[0] = $1;
                        $$->child[1] = $4;}
            | add_exp { $$ = $1;}
            ;

relop       : LTEQ {savedop = LTEQ;}
            | LT {savedop = LT;}
            | MT {savedop = MT;}
            | MTEQ { savedop = MTEQ;}
            | EQ { savedop = EQ;}
            | NEQ { savedop = NEQ;}
            ;

add_exp     : add_exp addop { $$ = newExpNode(OpK);
                              $$->attr.op = savedop;}
              term { $$ = $3;
                     $$->child[0] = $1;
                     $$->child[1] = $4;}
            |term { $$ = $1;}
            ;
addop       : PLUS {savedop = PLUS;}
            | MINUS {savedop = MINUS;}
            ;
term        : term mulop { $$ = newExpNode(OpK);
                           $$->attr.op = savedop;}
              factor { $$ = $3;
                       $$->child[0] = $1;
                       $$->child[1] = $4;}
            | factor { $$ = $1;}
            ;

/*
if_stmt     : IF exp THEN stmt_seq END
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            | IF exp THEN stmt_seq ELSE stmt_seq END
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                   $$->child[2] = $6;
                 }
            ;
repeat_stmt : REPEAT stmt_seq UNTIL exp
                 { $$ = newStmtNode(RepeatK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            ;
assign_stmt : ID { savedName = copyString(tokenString);
                   savedLineNo = lineno; }
              ASSIGN exp
                 { $$ = newStmtNode(AssignK);
                   $$->child[0] = $4;
                   $$->attr.name = savedName;
                   $$->lineno = savedLineNo;
                 }
            ;
read_stmt   : READ ID
                 { $$ = newStmtNode(ReadK);
                   $$->attr.name =
                     copyString(tokenString);
                 }
            ;
write_stmt  : WRITE exp
                 { $$ = newStmtNode(WriteK);
                   $$->child[0] = $2;
                 }
            ;
exp         : simple_exp LT simple_exp 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = LT;
                 }
            | simple_exp EQ simple_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = EQ;
                 }
            | simple_exp { $$ = $1; }
            ;
simple_exp  : simple_exp PLUS term 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = PLUS;
                 }
            | simple_exp MINUS term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MINUS;
                 } 
            | term { $$ = $1; }
            ;

term        : term TIMES factor 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = TIMES;
                 }
            | term OVER factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = OVER;
                 }
            | factor { $$ = $1; }
            ;
factor      : LPAREN exp RPAREN
                 { $$ = $2; }
            | NUM
                 { $$ = newExpNode(ConstK);
                   $$->attr.val = atoi(tokenString);
                 }
            | ID { $$ = newExpNode(IdK);
                   $$->attr.name =
                         copyString(tokenString);
                 }
            | error { $$ = NULL; }
            ;
*/

mulop       : TIMES { savedop = TIMES;}
            | OVER { savedop = OVER;}
            ;
factor      : LPAREN exp RPAREN { $$ = $2;}
            | var { $$ = $1;}
            | call { $$ = $1;}
            | NUM { $$ = newExpNode(ConstK);
                    $$->attr.val = atoi(tokenString);} 
            ;

call        : idsave LPAREN args RPAREN { $$ = newCallNode(CalllK);
                                          $$->child[0] = $1;
                                          $$->child[1] = $3;
                                          $$->lineno = lineno;}
            ;

args        : arg_list {$$ = $1;}
            | {$$ = NULL;}
            ;

arg_list    : arg_list CM exp { YYSTYPE t = $1;
                                if(t != NULL)
                                { while( t->sibling != NULL)
                                        t = t->sibling;
                                  t->sibling = $3;
                                  $$ = $1;}
                               else $$ = $3;}
            | exp { $$ = $1;}
            ;


%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");

  if(yychar == LT)fprintf(listing,"LT\t");
  if(yychar == EQ)fprintf(listing,"EQ\t");
  if(yychar == SEMI)fprintf(listing,"SEMI\t");
  if(yychar == PLUS)fprintf(listing,"PLUS\t"); 
  if(yychar == MINUS)fprintf(listing,"MINUS\t");
  if(yychar == TIMES)fprintf(listing,"TIMES\t");
  if(yychar == OVER)fprintf(listing,"OVER\t");
  if(yychar == LTEQ)fprintf(listing,"LTEQ\t");
  if(yychar == MT)fprintf(listing,"MT\t");
  if(yychar == MTEQ)fprintf(listing,"MTEQ\t");
  if(yychar == ASSIGN)fprintf(listing,"ASSIGN\t");
  if(yychar == NEQ)fprintf(listing,"NEQ\t");
  if(yychar == CM)fprintf(listing,"CM\t");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

