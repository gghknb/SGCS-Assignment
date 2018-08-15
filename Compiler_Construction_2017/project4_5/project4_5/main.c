/****************************************************/
/* File: main.c                                     */
/* Main program for C- Language compiler            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */

#if NO_PARSE
#include "scan.h"
#else
#if NO_ANALYZE
#include "scan.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#else
#include "scan.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#endif
#endif
#include "cgen.h"

#define FILELEN  100

/* allocate global variables */
int  g_line  = 0;       
FILE *g_src  = NULL;    
FILE *g_dest = NULL;    
FILE *code;

/* edit */
int assign_flag = 0;
int array_flag = 0;
int global_flag = 1;
int para_flag = 0;
int vara_flag = 0;
int sim_flag = 0;
int func_flag = 0;
int idd_flag = 0;

/* check flag */
int g_echoSrc = TRUE; 
int g_traceScan = FALSE;
int g_traceParse = FALSE;
int g_traceAnalyze = TRUE; 
int g_traceCode = FALSE;

int g_error = FALSE;

int main (int argc, char *argv[]) 
{   char pgm[105], temp[105];
	TokenType tmp;
	AST_node  *root;
	scopeHash *global = newScopeHash(0, 0, "global");

	if (argc != 2) {
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	strncpy(pgm, argv[1], FILELEN);
	if (!strchr(pgm, '.'))
		strcat(pgm, ".tny\0");

	strcpy(temp, pgm);
	strcat(temp, ".tmp");
	appendIOFunc(pgm, temp);

    g_src = fopen(temp, "r");
	if(g_src==NULL) {
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	g_dest = stdout;
#if NO_PARSE
	fprintf(g_dest, "%-10s%-10s%s\n", "line", "token", "lexeme");
	fprintf(g_dest, "-----------------------------------------------\n");
	do
		tmp = getToken();
	while (tmp != ENDFILE && tmp != ERROR);
#else
	root = parse();
	if (g_traceParse && !g_error) {
		fprintf(g_dest, "abstract syntax tree\n");
		fprintf(g_dest, "-----------------------------------------------\n");
		printTree(root);
	}
#if NO_ANALYZE
	destroyTree(root);
#else
	if (!g_error) {
		buildSymTab(global, root);
		if (!g_error)
			mainCheck(global);
		if (!g_error)
			typeCheck(global, root);
	}
	if (g_traceAnalyze && !g_error){
		start(global);
	//	printScopeHash(global);
	}
	//destroyTree(root);
	//destroyScopeHash(global);
#endif
#endif


	if (! g_error)
	{ char * codefile;
		int fnlen = strcspn(pgm,".");
		codefile = (char *) calloc(fnlen+4, sizeof(char));
		strncpy(codefile,pgm,fnlen);
		strcat(codefile,".s");
		code = fopen(codefile,"w");
		if (code == NULL)
		{ printf("Unable to open %s\n",codefile);
			exit(1);
		}
		codeGen(root,codefile,global);
		fclose(code);
	}
	fclose(g_src);
	return 0;
}
