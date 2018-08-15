#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "analyze.h"


static int find_ret_stmt(AST_node *root) {

	int i;

	if (!root)
		return 0;
    else if (root->kind == RETSTMT)
		return 1;

	for (i = 0; i < MAXCHILDREN; ++i) {
		if (find_ret_stmt(root->child[i]) != 0)
			return 1;
	}

	if (find_ret_stmt(root->sibling) != 0)
		return 1;

	return 0;
}

void start(scopeHash* current)
{
    int i;
    current = current->children;
    while (1)
    {
        if(!current) break;
        if(bsflag) {
            find(current, current);
            current = current->sibling;
        }
        else nnflag = 2;
	}
}
void find(scopeHash* save, scopeHash* current)
{
    int i;
    if (current == NULL) return;
    declInfo *ret; 
    ret = NULL;
    scopeHash * child;
    child = NULL;
    for (i = 0; i < HASH_SIZE; i++)
    {
        ret = current->decls[i];
        while (1)
		{
            if(!ret) break;
			if (save->max < ret->loc && bsflag)save->max = ret->loc;
			if (save->min > ret->loc && bsflag)save->min = ret->loc;
			ret = ret->link;
		}
	}
    if(bsflag) {
        current->max = save->max;
        current->min = save->min;
        child = current->children;
    }
    while (1)
    {
        if(!child) break;
        if(bsflag) {
            find(save,child);
            child = child->sibling;
        }
        else nnflag = 2;
    }
}

// build symbol table.
void buildSymTab(scopeHash *current, AST_node *root) {

	int i;
	auxInfo    tmpAux = {0, };
	declInfo  *tmpDecl;       
    tmpDecl = NULL;
	scopeHash *tmpScopeHash;  
    tmpScopeHash = NULL;
	scopeHash *nextScopeHash; 
    nextScopeHash = current;

	if ((!root || g_error) && stopflag)
		return;
	else if (root->kind == FUNDECL ) {                   
		if (searchHash(current, 0, root->attr.name) && buildflag) {  
            semanticError(root->attr.line, "multiple function declaration");
            return;
        }	
        if(buildflag) {
            tmpAux.fInfo.retType = root->child[0]->attr.what;
            tmpScopeHash = newScopeHash(-4, current->scope + 1, root->attr.name);
            appendChildScopeHash(current, tmpScopeHash);
        }
        if (root->child[1]->attr.what != VOID) {
            int cnt; 
            AST_node *tmp;
            cnt = 1;
            tmp = root->child[1];
            while (1) {
                if(!tmp) break;
                auxInfo paramInfo;
                if (searchHash(tmpScopeHash, 0, tmp->attr.name)) {  
                    semanticError(tmp->attr.line, "multiple parameter declaration");
                    return;
                }
                if (tmp->child[0]->attr.what == VOID) {             
                    semanticError(tmp->attr.line, "parameter have VOID type");
                    return;
                }

                appendParam(&tmpAux, tmp->subkind);
                if(tmp->subkind != ARR)
                {
                    paramInfo.vpInfo.isArr = 0;
                    paramInfo.vpInfo.arrSize = 1;
                }
                else
                {
                    paramInfo.vpInfo.isArr = 1;
                    paramInfo.vpInfo.arrSize = tmp->attr.length;
                }
                
                /*paramInfo.vpInfo.isArr   = (tmp->subkind != ARR)? 0 : 1;
                paramInfo.vpInfo.arrSize = (tmp->subkind != ARR)? 1 : tmp->attr.length;
               */ tmpDecl = newDeclInfo(tmp->attr.name, 4 * cnt, accum_loc + 4 * cnt + 4, P, paramInfo);
                cnt = cnt + 1;	
                appendLine(tmpDecl, tmp->attr.line);
                insertHash(tmpScopeHash, tmpDecl);
                tmp = tmp->sibling;

            }
        }

        tmpDecl = newDeclInfo(root->attr.name, funcNumber, funcNumber, F, tmpAux);
        funcNumber = funcNumber + 1;	
        appendLine(tmpDecl, root->attr.line);
        insertHash(current, tmpDecl);

        funcBodyFlag = 1;

        buildSymTab(tmpScopeHash, root->child[2]);
    }
	else if (root->kind == COMPSTMT ) {                  
		if (funcBodyFlag)
			funcBodyFlag = 0;
		else if(buildflag) {
			nextScopeHash = newScopeHash(current->fp, current->scope + 1, "noname");
			appendChildScopeHash(current, nextScopeHash);
		}
	}

	else if (root->kind == VARDECL ) {                   
		if (searchHash(current, 0, root->attr.name)) {  
			semanticError(root->attr.line,"multiple declaration\n");
			return;
		}
		if (root->child[0]->attr.what == VOID) {        
			semanticError(root->attr.line, "variable have VOID type");
			return;
		}
        if(root->subkind != ARR)
        {
            tmpAux.vpInfo.isArr = 0;
            tmpAux.vpInfo.arrSize = 1;
        }
        else
        {
            tmpAux.vpInfo.isArr = 1;
            tmpAux.vpInfo.arrSize = root->attr.length;
        }
/*
        tmpAux.vpInfo.isArr   = (root->subkind != ARR)? 0 : 1;
		tmpAux.vpInfo.arrSize = (root->subkind != ARR)? 1 : root->attr.length;
*/		
		current->fp +=  (!current->scope)? 4 * tmpAux.vpInfo.arrSize : -4 * tmpAux.vpInfo.arrSize;
		accum_loc   +=  (!current->scope)? 0 : -4 * tmpAux.vpInfo.arrSize;
		
		tmpDecl = newDeclInfo(root->attr.name, current->fp, (current->scope)? accum_loc : current->fp, V, tmpAux);
		appendLine(tmpDecl, root->attr.line);
		insertHash(current, tmpDecl);
	}
	else if ((root->kind == VAR || root->kind == CALL) ) { 
		tmpDecl = searchHash(current, 1, root->attr.name);
		if (!tmpDecl) {                                 
			semanticError(root->attr.line, "Variable not exist");
			return;
		}

		root->scope = current;
		appendLine(tmpDecl, root->attr.line);
	}
	if (root->kind != FUNDECL && buildflag) {
		for (i = 0; i < MAXCHILDREN; ++i)
			buildSymTab(nextScopeHash, root->child[i]); 
	}
	buildSymTab(current, root->sibling);                
}


void mainCheck(scopeHash *current) {

    declInfo *decl;
    decl = searchHash(current, 0, "main");
    if(bsflag) {
        if (!decl && mainflag)                                  
            semanticError(0, "Not have main function");
        else if (decl->loc + 1 != funcNumber && mainflag)       
            semanticError(decl->lines->line, "main function must be declared at the end");
        else if (decl->aux.fInfo.retType != VOID && mainflag)   
            semanticError(decl->lines->line, "main function should return void");
        else if (decl->aux.fInfo.params != NULL && mainflag)    
            semanticError(decl->lines->line, "main function parameter must be Void type");
    }
    return;
}


void typeCheck(scopeHash *current, AST_node *root) {

	int i;
	scopeHash *nextScopeHash;
    nextScopeHash = current;
    declInfo *target;
    declInfo *idx;
    AST_node *astIt;
    listNode *paramIt;

	if ((!root || g_error) && stopflag)
		return;
    switch(root->kind) {
    case RELOP :  
    case ADDOP :  
    case MULOP :      
		for (i = 0; i < 2; ++i) {
			if (root->child[i]->kind == CALL && type2flag != 0) {
				target = searchHash(current, 1, root->child[i]->attr.name);
				if(target->aux.fInfo.retType != INT) {
					semanticError(root->child[i]->attr.line, "if statement can be only expression or integer");
					return;
				}
			}
		}
      break;
    case SELSTMT :
    case ITERSTMT :                     
		if (root->child[0]->kind == CALL && type2flag != 0) {
			target = searchHash(current, 1, root->child[0]->attr.name);
			if(target->aux.fInfo.retType == VOID) {
				semanticError(root->child[0]->attr.line, "Loop condition can be only expression or integer");
				return;
			}
		}
    	break;
	case EXPR :                                                    
		
		if (root->child[1]->kind == CALL&& type2flag != 0) {
			target = searchHash(current, 1, root->child[1]->attr.name);
			if(target->vpf == F && target->aux.fInfo.retType != INT) {
				semanticError(root->child[1]->attr.line, "Error in Op / Assign");
				return;
			}
		}
    	break;
	case VAR:                                                    
		target = searchHash(current, 1, root->attr.name);
		
		if (target->vpf == F && type2flag != 0) {
			semanticError(root->attr.line, "invalid variable usage. function used as variable");
			return;
		}	
		if (!target->aux.vpInfo.isArr && root->subkind == ARR) {
			semanticError(root->attr.line, "Not using variable like array");
			return;
		}
		if ( target->aux.vpInfo.isArr && root->subkind == SOL && !root->attr.par) {
			semanticError(root->attr.line, "Not using variable like array");
			return;
		}
		if (root->subkind == ARR && root->child[0]->kind == CALL) {
			idx = searchHash(current, 1, root->child[0]->attr.name);
			if (idx->vpf == F && idx->aux.fInfo.retType == VOID) {
				semanticError(root->attr.line, "array index must be number");
				return;
			}
		}
	    break;
	case CALL:                                                  
		target = searchHash(current, 1, root->attr.name);
		
		if (target->vpf != F&& type2flag != 0) {
			semanticError(root->attr.line, "Variable or parameter can`t be use like function");
			return;
		}
		
		astIt   = root->child[0];
		paramIt = target->aux.fInfo.params;
		while (1) {
            if(!(astIt && paramIt)) break;
			if (paramIt->isArr) {
				declInfo *var;
				if (astIt->kind != VAR || astIt->child[0]) {
					semanticError(astIt->attr.line, "Invalid parameter usage");
					return;
				}
				var = searchHash(current, 1, astIt->attr.name);
				if (!var->aux.vpInfo.isArr) {
					semanticError(astIt->attr.line, "Invalid parameter usage");
					return;
				}
			}
			
			else if(type2flag) {
				if (astIt->kind == VAR) {
					declInfo *var = searchHash(current, 1, astIt->attr.name);
					if (var->aux.vpInfo.isArr && !astIt->child[0]) {
						semanticError(root->attr.line, "Invalid parameter usage.");
						return;
					}
				}
				else if (astIt->kind == CALL) {
					declInfo *fun = searchHash(current, 1, astIt->attr.name);
					if (fun->aux.fInfo.retType == VOID) {
						semanticError(astIt->attr.line, "Invalid parameter usage.");
						return;
					}
				}
			}
			astIt   = astIt->sibling;
			paramIt = paramIt->link;
		}
		
		if (astIt || paramIt) {
			semanticError(root->attr.line, "call function argument do not match with decl function parameter");
			return;
		}
        break;
	case FUNDECL :                                                
		target = searchHash(current, 1, root->attr.name);
		
		if (target->aux.fInfo.retType == INT&& type2flag != 0) {
			AST_node *astIt = root->child[2]->child[1];
			while (1) {
                if(!astIt) break;
				if (astIt->kind == RETSTMT) {
					if (!astIt->child[0]) {
						semanticError(astIt->attr.line, "Return should be INT");
						return;
					}
					else if (astIt->child[0]->kind == CALL) {
						declInfo *fun = searchHash(current, 1, astIt->child[0]->attr.name);
						if (fun->aux.fInfo.retType == VOID) {
							semanticError(astIt->attr.line, "Return should be INT");
							return;
						}
					}
				}
				astIt = astIt->sibling;
			}
			if (!find_ret_stmt(root->child[2]->child[1])) {
				semanticError(root->attr.line, "Return is necessary");
				return;
			}
		}
		else if (target->aux.fInfo.retType == VOID&& type2flag != 0) {
	        astIt = root->child[2]->child[1];
			while (1) {
                if(!astIt) break;
				if (astIt->kind == RETSTMT && astIt->child[0]) {
					semanticError(astIt->attr.line, "Return not required");
					return;
				}
				astIt = astIt->sibling;
			}
		}
	    break;
    case COMPSTMT :                                                 
		nextScopeHash = getUnvisitedChildScopeHash(current);
        break;
    default : break;
    }
 
	for (i = 0; i < MAXCHILDREN; ++i)
		typeCheck(nextScopeHash, root->child[i]);     
	typeCheck(current, root->sibling);                
}

void semanticError(int line, const char *message) {
    if(bsflag) {
        printf("Error in line %d : ", line);
        printf(" %s\n", message);
        g_error = 1;
    }
}


