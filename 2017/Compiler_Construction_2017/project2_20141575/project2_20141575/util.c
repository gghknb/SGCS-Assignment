/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

int array_flag = 0;
int assign_flag = 0;
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
    case PLUS: fprintf(listing,"+\t\t%s\n",tokenString); break; // +
    case MINUS: fprintf(listing,"-\t\t%s\n",tokenString); break; // -
    case TIMES: fprintf(listing,"*\t\t%s\n",tokenString ); break; // *
    case OVER: fprintf(listing,"/\t\t%s\n",tokenString); break; // / 
    case MT: fprintf(listing,">\t\t%s\n",tokenString); break; // >
    case MTEQ: fprintf(listing,">=\t\t%s\n",tokenString); break; // >=
    case LT: fprintf(listing,"<\t\t%s\n",tokenString); break;   // <
    case LTEQ: fprintf(listing,"<=\t\t%s\n",tokenString); break;   // <=
    case EQ: fprintf(listing,"==\t\t%s\n",tokenString); break;  // ==
    case NEQ: fprintf(listing,"!=\t\t%s\n",tokenString); break;  // !=
    case ASSIGN: fprintf(listing,"=\t\t%s\n",tokenString); break; // =
    case SEMI: fprintf(listing,";\t\t%s\n",tokenString); break;  // ;
    case CM: fprintf(listing,",\t\t%s\n",tokenString); break;  // ,
    case LPAREN: fprintf(listing,"(\t\t%s\n",tokenString); break; // (
    case RPAREN: fprintf(listing,")\t\t%s\n",tokenString); break; // )
    case LSB: fprintf(listing,"[\t\t%s\n",tokenString); break; // [
    case RSB: fprintf(listing,"]\t\t%s\n",tokenString); break; // ]
    case LCB: fprintf(listing,"{\t\t%s\n",tokenString); break; // {
    case RCB: fprintf(listing,"}\t\t%s\n",tokenString); break; // } 
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM: fprintf(listing,"NUM\t\t%s\n",tokenString);  break;
    case ID: fprintf(listing,"ID\t\t%s\n",tokenString); break;
    case ERROR: fprintf(listing,"ERROR\t\t%s\n",tokenString); break;
    case COMMENTS: fprintf(listing,"ERROR\t\tComment Error\n"); break;
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

TreeNode *newDeclNode(DeclKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t == NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for(i = 0 ; i < MAXCHILDREN;i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DeclK;
        t->kind.decl = kind;  
        t->lineno = lineno;
        
    }
    return t;
}
TreeNode *newParaNode(ParaKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t == NULL)
        fprintf(listing,"ERROR\n");
    else
    {
        for(i = 0 ; i < MAXCHILDREN;i++)        
             t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ParaK;
        t->kind.decl = kind;
        t->lineno = lineno;
        
    }
}
TreeNode *newCallNode(CallKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t == NULL)
        fprintf(listing,"ERROR\n");
    else
    {
        for(i = 0 ; i < MAXCHILDREN;i++)
               t->child[i] = NULL;          
        t->sibling = NULL;
        t->nodekind = CallK;
        t->kind.decl = kind;
        t->lineno = lineno;
        
    }
}
TreeNode *newTypeNode(TypeKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t == NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for(i = 0 ; i < MAXCHILDREN;i++)
           t->child[i] = NULL;

        t->sibling = NULL;
        t->nodekind = TypeK;
        t->kind.decl = kind;
        t->lineno = lineno;
        
    }
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
static int indentno = 0;

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
   // printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          printSpaces();
          fprintf(listing,"If\n");
          break;
        case ReturnK:
          printSpaces();
          fprintf(listing,"Return\n");
          break;
        case WhileK:
          printSpaces();
          fprintf(listing,"While\n");
          break;
        case IfelK:
          printSpaces();
          fprintf(listing,"If Else\n");
          break;
        case CompK:
          printSpaces();
          fprintf(listing,"Compound statement\n");
          break;
        case ReturnexpK:
          printSpaces();
          fprintf(listing,"Return and expression\n");
          break;
        default:
          printSpaces();
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          printSpaces();
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          printSpaces();        
          fprintf(listing,"Const: %d\n",tree->attr.val); 
          break;
        case IdK:
          if(assign_flag == 1 && array_flag == 0)
          {
              fprintf(listing,"%s\n",tree->attr.name);
              assign_flag = 0;
          }
          else if(assign_flag == 1 && array_flag == 1)
          {
               fprintf(listing,"%s\n",tree->attr.name);
               assign_flag = 0;
          }
          else {
             printSpaces();
             fprintf(listing,"Id: %s\n",tree->attr.name);
          }
          array_flag = 0;
          break;
        case ArrK:
          if(assign_flag == 0) printSpaces();
          fprintf(listing,"Array\n");
          array_flag = 1;
          assign_flag = 0;
          break;
        case SimpleK:
             printSpaces();
        
          fprintf(listing,"Simple expression\n");
          INDENT;
          printSpaces();
          UNINDENT;
          fprintf(listing,"OP: ");
          printToken(tree->attr.op,"\0");
          break;
        case EpK:
          printSpaces();
          fprintf(listing,"Assign to : ");
          assign_flag = 1;
          break;
        default:
             printSpaces();
        
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if(tree->nodekind == TypeK)
    {     printSpaces();
        
        switch(tree->kind.type) {
        case IntK:
            fprintf(listing,"Type : INT\n");
            break;
        case VoidK:
            fprintf(listing,"Type : VOID\n");
            break;
        }
    }
    else if(tree->nodekind == CallK)
    {    printSpaces();
        
        if(tree->kind.call == CalllK)
            fprintf(listing,"Call\n");
    }
    else if(tree->nodekind == ParaK)
    {    printSpaces();
        
        switch(tree->kind.para) {
        case PidK:
            fprintf(listing,"Parameter\n");
            break;
        case ParK:
            fprintf(listing,"Parameter array\n");
            break;
       }
    }
    else if(tree->nodekind == DeclK)
    {     printSpaces();
        
        switch(tree->kind.decl) {
        case VarK:
             fprintf(listing,"Variable Declaration\n");
             break;
        case VarrK:
             fprintf(listing,"Array Declaration\n");
             break;
        case FuncK:
             fprintf(listing,"Function Declaration\n");
             break;
        }
    }
    else{
        fprintf(listing,"Unknown node kind\n");
        printSpaces();
        printSpaces();
    }          
    for (i=0;i<MAXCHILDREN;i++)
        printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}

