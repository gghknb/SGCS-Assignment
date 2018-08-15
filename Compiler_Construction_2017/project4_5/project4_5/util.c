/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C- Language compiler                     */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
static int level = 0;
static int st_flag = 0;

void printToken(TokenType token, const char *tokenString) {
    switch (token) {
        case IF:  fprintf(g_dest, "%-10s%s\n", "IF", tokenString); break;
        case ELSE:fprintf(g_dest, "%-10s%s\n", "ELSE", tokenString); break;
        case INT: fprintf(g_dest, "%-10s%s\n", "INT", tokenString); break;
        case VOID: fprintf(g_dest, "%-10s%s\n", "VOID", tokenString);break;
        case WHILE: fprintf(g_dest, "%-10s%s\n", "WHILE", tokenString); break;
        case RETURN: fprintf(g_dest, "%-10s%s\n", "RETURN", tokenString); break;
        case ID: fprintf(g_dest, "%-10s%s\n", "ID", tokenString); break;
        case NUM: fprintf(g_dest, "%-10s%s\n", "NUM", tokenString); break;
        case ASSIGN: fprintf(g_dest, "%-10s%s\n", "=", tokenString);break;
        case SEMI: fprintf(g_dest, "%-10s%s\n", ";", tokenString); break;
        case COMMA: fprintf(g_dest, "%-10s%s\n", ",", tokenString);break;
        case PLUS: fprintf(g_dest, "%-10s%s\n", "+", tokenString); break;
        case MINUS:fprintf(g_dest, "%-10s%s\n", "-", tokenString); break;
        case MUL: fprintf(g_dest, "%-10s%s\n", "*", tokenString); break;
        case DIV: fprintf(g_dest, "%-10s%s\n", "/", tokenString); break;
        case LT: fprintf(g_dest, "%-10s%s\n", "<", tokenString); break;
        case LET:fprintf(g_dest, "%-10s%s\n", "<=", tokenString);break;
        case GT: fprintf(g_dest, "%-10s%s\n", ">", tokenString); break;
        case GET:fprintf(g_dest, "%-10s%s\n", ">=", tokenString);break;
        case EQ: fprintf(g_dest, "%-10s%s\n", "==", tokenString);break;
        case NEQ:fprintf(g_dest, "%-10s%s\n", "!=", tokenString);break;
        case LPRTS: fprintf(g_dest, "%-10s%s\n", "(", tokenString); break;
        case RPRTS: fprintf(g_dest, "%-10s%s\n", ")", tokenString); break;
        case LBRC: fprintf(g_dest, "%-10s%s\n", "{", tokenString); break;
        case RBRC: fprintf(g_dest, "%-10s%s\n", "}", tokenString); break;
        case LBRKT: fprintf(g_dest, "%-10s%s\n", "[", tokenString); break;
        case RBRKT: fprintf(g_dest, "%-10s%s\n", "]", tokenString); break;
        case ENDFILE: fprintf(g_dest, "%-10s\n", "EOF"); break;
        default: fprintf(g_dest, "%-10s%s\n", "ERROR", tokenString); break;
    }
}

AST_node* newASTNode(NodeKind kind, SubKind sub) { // create AST node
    AST_node *ret = (AST_node *)calloc(1, sizeof(AST_node));
    if (!ret) {
        fprintf(g_dest, "[compiler] calloc failed.");
        exit(1);
    }
    ret->kind = kind;
    ret->subkind = sub;
    ret->attr.line = g_line;
	ret->scope = NULL;
    return ret;
}

char* allocString(char *src) {
    char *ret = NULL;
    int flag = 1;

    if (!src) return NULL;
    ret = (char *)calloc(strlen(src) + 1, sizeof(char));
    if ((!ret) && flag) {
        fprintf(g_dest, "[compiler] calloc failed.");
        exit(1);
    }
    strcpy(ret, src);

    return ret;
}

void appendIOFunc(const char *fname, const char *fname2) {
	char tmp;
	FILE *fp_read = NULL, *fp_write = NULL;

	if(!(fp_read = fopen(fname, "r"))) {
		fprintf(stderr, "Error! : File %s not found\n", fname);
		exit(1);
	}
	if(!(fp_write = fopen(fname2, "w"))) {
		fprintf(stderr, "Error! : Can't write\n");
		exit(1);
	}

	fprintf(fp_write, "int input (void) { return 1; }\nvoid output (int a) { return; }\n");
	while (fscanf(fp_read, "%c", &tmp) != EOF)
		fprintf(fp_write, "%c", tmp);
	
    fclose(fp_write);
	fclose(fp_read);
}

void printSpace(int level) {
    int i;

    for(i=level; i>0; i--){
        fprintf(g_dest, "  ");
    }
}

void printTree(AST_node *root) {
    int i;
    while (root) {
        switch (root->kind) {
            case ITERSTMT:
                printSpace(level);
                fprintf(g_dest, "Iteration Statement.\n");
                break;
            case COMPSTMT:
                printSpace(level);
                fprintf(g_dest, "Compound Statement.\n");
                break;
            case RETSTMT:
                printSpace(level);
                fprintf(g_dest, "Return Statement.\n");
                break;
            case SELSTMT:
                printSpace(level);
                fprintf(g_dest, "Selection Statement.\n");
                break;
            case VARDECL:
                printSpace(level);
                if (level == 0)
                    fprintf(g_dest, "Variable Declaration(global).\n");
                else
                    fprintf(g_dest, "Variable Declaration.\n");
                if (root->subkind == SOL) {
                    printSpace(level);
                    fprintf(g_dest, "var id         : %s\n", root->attr.name);
                }
                else if (root->subkind == ARR) {
                    printSpace(level);
                    fprintf(g_dest, "var id(array)  : %s\n", root->attr.name);
                    printSpace(level);
                    fprintf(g_dest, "var length     : %d\n", root->attr.length);
                }
                break;
            case FUNDECL:
                printSpace(level);
                fprintf(g_dest, "Function Declaration.\n");
                printSpace(level);
                fprintf(g_dest, "fun id         : %s\n", root->attr.name);
                break;           case VAR:
                printSpace(level);
                fprintf(g_dest, "Variable.\n");
                printSpace(level);
                if (root->subkind == SOL)
                    fprintf(g_dest, "var id         : %s\n", root->attr.name);
                else if (root->subkind == ARR)
                    fprintf(g_dest, "var id(array)  : %s\n", root->attr.name);
                break;
            case PARAM:
                printSpace(level);
                fprintf(g_dest, "Parameter.\n");
                printSpace(level);
                if (root->subkind == SOL)
                    fprintf(g_dest, "param id       : %s\n", root->attr.name);
                else if (root->subkind == ARR)
                    fprintf(g_dest, "param id(array): %s\n", root->attr.name);
                break;
            case PARAMS:
                printSpace(level);
                fprintf(g_dest, "Parameter.\n");
                printSpace(level);
                fprintf(g_dest, "param          : VOID\n");
                break;
            case CALL:
                printSpace(level);
                fprintf(g_dest, "Function Call.\n");
                printSpace(level);
                fprintf(g_dest, "fun id         : %s\n", root->attr.name);
                break;
            case EXPR:
                printSpace(level);
                fprintf(g_dest, "Expression.\n");
                break;
            case FACTOR:
                printSpace(level);
                fprintf(g_dest, "Factor.\n");
                printSpace(level);
                fprintf(g_dest, "factor value   : %d\n", root->attr.val);
                break;
            case RELOP:
                printSpace(level);
                fprintf(g_dest, "Relational Expression.\n");
                printSpace(level);
                switch (root->attr.what) {
                    case LT: fprintf(g_dest, "operator       : <\n"); break;
                    case LET:fprintf(g_dest, "operator       : <=\n");break;
                    case GT: fprintf(g_dest, "operator       : >\n"); break;
                    case GET:fprintf(g_dest, "operator       : >=\n");break;
                    case EQ: fprintf(g_dest, "operator       : ==\n");break;
                    case NEQ:fprintf(g_dest, "operator       : !=\n");break;
                }
                break;
            case ADDOP:
                printSpace(level);
                fprintf(g_dest, "Additive Expression.\n");
                printSpace(level);
                switch (root->attr.what) {
                    case PLUS:  fprintf(g_dest, "operator       : +\n"); break;
                    case MINUS: fprintf(g_dest, "operator       : -\n"); break;
                }
                break;
            case MULOP:
                printSpace(level);
                fprintf(g_dest, "Multiplicative Expression.\n");
                printSpace(level);
                switch (root->attr.what) {
                    case DIV: fprintf(g_dest, "operator       : /\n"); break;
                    case MUL: fprintf(g_dest, "operator       : *\n"); break;
                }
                break;
            case TYPESPEC:
                printSpace(level);
                fprintf(g_dest, "Type Specification.\n");
                printSpace(level);
                if (root->attr.what == INT)
                    fprintf(g_dest, "type           : INT\n");
                else if (root->attr.what == VOID)
                    fprintf(g_dest, "type           : VOID\n");
                break;
        }
        level++;
        for (i = 0; i < MAXCHILDREN; ++i)
            printTree(root->child[i]);
        root = root->sibling;
        level--;
    }
}

void destroyTree(AST_node *source) {
    int i;
    AST_node *temp;
    temp = source;

    while (source) {
        for (i = 0; i < MAXCHILDREN; ++i)
            destroyTree(source->child[i]);
        temp = source;
        if (temp->attr.name)
            free(temp->attr.name);
        free(temp);
        source = source->sibling;
    }
}

void printScopeHash(scopeHash *global) {
    listNode *lineNode;
    int i;

    st_flag = 0;
    if (!global)
        return;
    printf("%10s%10s%10s%10s%10s%10s%10s%10s%10s\t%10s\n",
            "Name", "Sname", "Scope", "Loc", "Accum", "V/P/F", "Array?", "Size", "Type", "Line Numbers");
	printf(" ");
    for(i=0; i<120; i++)
        printf("-");
    printf("\n");

    for (i = 0; i < HASH_SIZE; ++i) {
        if (global->decls[i]) {
            declInfo *tmp = global->decls[i];
            while (tmp) {
                lineNode = tmp->lines;
                st_flag++;
                printf("%10s%10s%10d%10d%10d%10d",
						tmp->id, global->name, global->scope, tmp->loc, tmp->accum_loc, tmp->vpf);
                if (tmp->vpf == V) {
                    printf("%10d%10d", tmp->aux.vpInfo.isArr, tmp->aux.vpInfo.arrSize);
                    if (tmp->aux.vpInfo.isArr) printf("%10s", "array");
                    else printf("%10s", "int");
                }
                else if (tmp->vpf == P) {
                    printf("%10d%10s", tmp->aux.vpInfo.isArr, "-");
                    if (tmp->aux.vpInfo.isArr) printf("%10s", "array");
                    else printf("%10s", "int");
                }
                else if (tmp->vpf == F) {
                    printf("%10s%10s", "-", "-");
                    if (tmp->aux.fInfo.retType == INT) printf("%10s", "int");
                    else if (tmp->aux.fInfo.retType == VOID) printf("%10s", "void");
                }
                if (lineNode) {
                    printf("%10d", lineNode->line);
                    lineNode = lineNode->link;
                }
                while (lineNode) {
                    printf("%5d", lineNode->line);
                    lineNode = lineNode->link;
                }
                printf("\n");
                tmp = tmp->link;
            }
        }
    }
    printf("\n");
    printScopeHash(global->children);
    printScopeHash(global->sibling);
}

void destroyScopeHash(scopeHash *global) {
    declInfo *declIt, *declTmp;
    listNode *listIt, *listTmp;
    int i;

    if (global->sibling) destroyScopeHash(global->sibling);
    if (global->children) destroyScopeHash(global->children);

    for (i = 0; i < HASH_SIZE; ++i) {
        if (global->decls[i]) {
            declIt = global->decls[i];
            while (declIt) {
                declTmp = declIt;
                declIt  = declIt->link;
                free(declTmp->id);          
                if (declTmp->vpf == F) {
                    listIt = declTmp->aux.fInfo.params;
                    while (listIt) {
                        listTmp = listIt;
                        listIt  = listIt->link;
                        free(listTmp);      
                    }
                }
                listIt = declTmp->lines;
                while (listIt) {
                    listTmp = listIt;
                    listIt  = listIt->link;
                    free(listTmp);          
                }
                free(declTmp);              
            }
        }
    }

    /* free */
	free(global->name);
    free(global);
}

scopeHash* newScopeHash(int fp, int scope, char *name) {
    scopeHash *ret = (scopeHash *)calloc(1, sizeof(scopeHash));

	ret->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(ret->name, name);
    ret->scope = scope;
    ret->fp = fp;

    return ret;
}

void appendChildScopeHash(scopeHash *current, scopeHash *child) {
    child->parent = current;
    if (!current->children)
        current->children = child;
    else {
        scopeHash *tmp = current->children;
        while (tmp->sibling)
            tmp = tmp->sibling;
        tmp->sibling = child;
    }
}

scopeHash* getUnvisitedChildScopeHash(scopeHash *current) {
    scopeHash *temp;
    
    temp = current->children;
    while (temp) {
        if (temp->visit == 0) {
            temp->visit = 1;
            return temp;
        }
        temp = temp->sibling;
    }
    return temp;
}

declInfo* newDeclInfo(char *id, int loc, int accum_loc, VPF vpf, auxInfo aux) {
    declInfo *ret;

    ret = (declInfo *)calloc(1, sizeof(declInfo));
    ret->id = (char *)calloc(strlen(id) + 1, sizeof(char));
    strcpy(ret->id, id);
    ret->vpf = vpf;
    ret->aux = aux;
    ret->loc = loc;
    ret->accum_loc = accum_loc;

    return ret;
}

void appendLine(declInfo *decl, int line) {
    listNode *newNode = (listNode *)calloc(1, sizeof(listNode));
    newNode->line = line;

    if (!decl->lines)
        decl->lines = newNode;
    else {
        listNode *tmp = decl->lines;
        while (tmp->link) {
            if (tmp->line == line) {
                free(newNode);
                return;
            }
            tmp = tmp->link;
        }
        if (tmp->line == line) {
            free(newNode);
            return;
        }
        tmp->link = newNode;
    }
}

void appendParam(auxInfo *aux, int isArr) {
    listNode *newNode, *temp;

    newNode = (listNode *)calloc(1, sizeof(listNode));
    if(isArr == ARR)
        newNode->isArr = 1;
    else
        newNode->isArr = 0;

    if (!aux->fInfo.params)
        aux->fInfo.params = newNode;
    else {
        temp = aux->fInfo.params;
        while (temp->link)
            temp = temp->link;
        temp->link = newNode;
    }
}

declInfo* searchHash(scopeHash *current, int mode, char *id) {
    declInfo *ret = NULL;
    int bucketIdx = hashFunc(id);

    while (current) {
        if (current->decls[bucketIdx]) {
            ret = current->decls[bucketIdx];
            while (ret) {
                if (strcmp(ret->id, id) == 0)
                    return ret;
                ret = ret->link;
            }
        }
        if (!mode) break;
        current = current->parent;
    }
    return ret;
}

scopeHash* searchScopeByFunc(scopeHash *current, char *name) {
	scopeHash *ret = NULL;

	if (strcmp(current->name, name) == 0)
		return current;
	if (current->sibling)
		ret = searchScopeByFunc(current->sibling, name);
	if (!ret && current->children)
		ret = searchScopeByFunc(current->children, name);

	return ret;
}

scopeHash* searchParentFuncScope(scopeHash *current) {
	while(current->parent && current->parent->parent)
		current = current->parent;
	return current;
}

int hashFunc(char *str) {
    int len, result;
    len = strlen(str);
    result = (str[0] * str[0] * str[0] + str[len / 2] * str[len / 2] + str[len - 1]) % HASH_SIZE;
    
    return result;
}

void insertHash(scopeHash *current, declInfo *decl) {
    int bucketIdx = hashFunc(decl->id);
    declInfo *temp;

    if (!current->decls[bucketIdx])
        current->decls[bucketIdx] = decl;
    else {
        temp = current->decls[bucketIdx];
        while (temp->link)
            temp = temp->link;
        temp->link = decl;
    }
}
