/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C- Language compiler                     */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */

void printOp(TokenType token){
	switch (token){
		case ASSIGN:    fprintf(listing, "="); break;
		case SEMI:  fprintf(listing, ";"); break;
		case PLUS:  fprintf(listing, "+"); break;
		case MINUS: fprintf(listing, "-"); break;
		case TIMES: fprintf(listing, "*"); break;
		case OVER:  fprintf(listing, "/"); break;
		case GT:    fprintf(listing, ">"); break;
		case GTE:  fprintf(listing, ">="); break;
		case LT:    fprintf(listing, "<"); break;
		case LTE:  fprintf(listing, "<="); break;
		case EQ:  fprintf(listing, "=="); break;
		case NEQ:	fprintf(listing, "!="); break;
	}
	fprintf(listing, "\n");
}
void printToken( TokenType token, const char* tokenString )
{ int i;
  switch (token){ 
    case IF: 
        fprintf(listing, "%-15s%-15s\n", "IF", tokenString); break;
	case ELSE:
        fprintf(listing, "%-15s%-15s\n", "ELSE", tokenString); break;
	case INT:
        fprintf(listing, "%-15s%-15s\n", "INT", tokenString); break;
	case RETURN:
        fprintf(listing, "%-15s%-15s\n", "RETURN", tokenString); break;
	case VOID:
        fprintf(listing, "%-15s%-15s\n", "VOID", tokenString); break;
	case WHILE:
        fprintf(listing, "%-15s%-15s\n", "WHILE", tokenString); break;
    case ASSIGN: 
    case SEMI:
    case PLUS:
    case MINUS:
    case TIMES:
    case OVER:
	case GT:
	case GTE:
    case LT: 
	case LTE:
	case EQ:
	case NEQ:
    case LPAREN:
    case RPAREN:
	case LBRACK:
	case RBRACK:
	case LBRACE:
	case RBRACE:
	case COMMA:
	  fprintf(listing, "%-15s%-15s\n", tokenString, tokenString); break;
	case COMERROR:
	  fprintf(listing, "%-15s%-15s\n", "ERROR", "Comment Error"); break;
	case ENDFILE: fprintf(listing, "%-30s\n", "EOF"); break;
    case NUM:
      fprintf(listing, "%-15s%-15s\n","NUM", tokenString);
      break;
    case ID:
      fprintf(listing, "%-15s%-15s\n","ID", tokenString);
      break;
    case ERROR:
      fprintf(listing, "%-15s%-15s\n", "ERROR", tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newDecNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newTypeNode(TypeKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = TypeK;
    t->kind.type = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newDecNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newDecNode(DecKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DecK;
    t->kind.dec = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newStmtNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
	if (tree->nodekind==TypeK){
		printf("Type : %s\n", tree->attr.name);
	}
	else if (tree->nodekind==DecK){
		switch (tree->kind.dec){
			case VarK:
				printf("Variable Declaration"); break;
			case ArrK:
				printf("Array Declaration"); break;
			case FunK:
				printf("Function Declaration"); break;
		}
		printf(" (%s)\n", tree->scope==Global ? "Global" : (tree->scope==Local? "Local":"Parameter"));
	}
	else if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case CompK:
			printf("Compound Statement\n"); break;
        case RetK:
			printf("Return Statement\n"); break;
        case ExprK:
			printf("Expression Statement\n"); break;
        case SelecK:
			printf("Selection Statement\n"); break;
        case IterK:
			printf("Iteration Statement\n"); break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n"); break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op : ");
          printOp(tree->attr.op);
          break;
        case ConstK:
          fprintf(listing,"Const : %d\n",tree->attr.val); break;
        case IdK:
          fprintf(listing,"Id : %s\n",tree->attr.name);
		  if (tree->range != 0){
			  printSpaces();
			  fprintf(listing, "Array Size : %d\n", tree->range);
		  }
          break;
		case AssignK:
		  fprintf(listing,"Assign to : %s\n", tree->attr.name); break;
		case CallK:
		  fprintf(listing,"Call Function : %s\n", tree->attr.name); break;
		case ArgK:
			printf("Arguments\n"); break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n"); break;
      }
    }
    else 
        fprintf(listing,"Unknown node kind\n");

    for (i=0;i<MAXCHILDREN;i++){
	
		if (tree->child[i] != NULL){
			if (tree->nodekind == StmtK && (tree->kind.stmt == SelecK || tree->kind.stmt == IterK))
				printSpaces();
			if (tree->nodekind == StmtK && tree->kind.stmt == SelecK && i == 0)
				fprintf(listing, "IF\n");
			if (tree->nodekind == StmtK && tree->kind.stmt == SelecK && i == 1)
				fprintf(listing, "THEN\n");
			if (tree->nodekind == StmtK && tree->kind.stmt == SelecK && i == 2)
				fprintf(listing, "ELSE\n");
			if (tree->nodekind == StmtK && tree->kind.stmt == IterK && i == 0)
				fprintf(listing, "WHILE\n");
			if (tree->nodekind == StmtK && tree->kind.stmt == IterK && i == 1)
				fprintf(listing, "DO\n");
			if (tree->nodekind == ExpK && (tree->kind.exp == IdK || tree->kind.exp == AssignK) && i == 0){
				printSpaces(); fprintf(listing, "INDEX\n");
			}
		}
		printTree(tree->child[i]);

	}
	tree = tree->sibling;
  }
  UNINDENT;
}
