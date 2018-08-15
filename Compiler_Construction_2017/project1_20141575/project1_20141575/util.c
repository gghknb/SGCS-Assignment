/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ 
  switch (token)
  { 
    case IF: fprintf(listing,"\tIF\t\t%s\n",tokenString); break;
    //case THEN:
    case ELSE:  fprintf(listing,"\tELSE\t\t%s\n",tokenString); break;
    case INT :  fprintf(listing,"\tINT\t\t%s\n",tokenString); break;
    case RETURN : fprintf(listing,"\tRETURN\t\t%s\n",tokenString); break;
    case VOID : fprintf(listing,"\tVOID\t\t%s\n",tokenString); break;
    case WHILE : fprintf(listing,"\tWHILE\t\t%s\n",tokenString); break;
    //case END:
    //case REPEAT:
    //case UNTIL:
    //case READ:
    //case WRITE:
    case PLUS: fprintf(listing,"\t+\t\t%s\n",tokenString); break; // +
    case MINUS: fprintf(listing,"\t-\t\t%s\n",tokenString); break; // -
    case TIMES: fprintf(listing,"\t*\t\t%s\n",tokenString ); break; // *
    case OVER: fprintf(listing,"\t/\t\t%s\n",tokenString); break; // / 
    case MT: fprintf(listing,"\t>\t\t%s\n",tokenString); break; // >
    case MTEQ: fprintf(listing,"\t>=\t\t%s\n",tokenString); break; // >=
    case LT: fprintf(listing,"\t<\t\t%s\n",tokenString); break;   // <
    case LTEQ: fprintf(listing,"\t<=\t\t%s\n",tokenString); break;   // <=
    case EQ: fprintf(listing,"\t==\t\t%s\n",tokenString); break;  // ==
    case NEQ: fprintf(listing,"\t!=\t\t%s\n",tokenString); break;  // !=
    case ASSIGN: fprintf(listing,"\t=\t\t%s\n",tokenString); break; // =
    case SEMI: fprintf(listing,"\t;\t\t%s\n",tokenString); break;  // ;
    case CM: fprintf(listing,"\t,\t\t%s\n",tokenString); break;  // ,
    case LPAREN: fprintf(listing,"\t(\t\t%s\n",tokenString); break; // (
    case RPAREN: fprintf(listing,"\t)\t\t%s\n",tokenString); break; // )
    case LSB: fprintf(listing,"\t[\t\t%s\n",tokenString); break; // [
    case RSB: fprintf(listing,"\t]\t\t%s\n",tokenString); break; // ]
    case LCB: fprintf(listing,"\t{\t\t%s\n",tokenString); break; // {
    case RCB: fprintf(listing,"\t}\t\t%s\n",tokenString); break; // } 
    case ENDFILE: fprintf(listing,"\tEOF\n"); break;
    case NUM: fprintf(listing,"\tNUM\t\t%s\n",tokenString);  break;
    case ID: fprintf(listing,"\tID\t\t%s\n",tokenString); break;
    case ERROR: fprintf(listing,"\tERROR\t\t%s\n",tokenString); break;
    case COMMENTS: fprintf(listing,"\tERROR\t\tComment Error\n"); break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
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
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case RepeatK:
          fprintf(listing,"Repeat\n");
          break;
        case AssignK:
          fprintf(listing,"Assign to: %s\n",tree->attr.name);
          break;
        case ReadK:
          fprintf(listing,"Read: %s\n",tree->attr.name);
          break;
        case WriteK:
          fprintf(listing,"Write\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}