/****************************************************/
/* File: code.h                                     */
/* Code emitting utilities for the TINY compiler    */
/* and interface to the SPIM machine                */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

/* flag for project4 */
static int factflag = 1;
static int returnflag = 1;
static int callflag = 1;
static int expflag =1;

/* code emitting utilities */

/* Procedure emitComment prints a comment line
 * with comment c in the code file
 */
void emitComment( char * c );
void emitComments(char * c, char* s);
void emit_one(int labelNum);
void emit_two(char *op, vn x, vn y, int type, char *c);
void emit_three(char *op, vn x, vn y, vn z, int type, char *c);
void emit_global(char* str, int type);
void emit_function(char* name);

void Selendemit();
void Selemit();
void Iterendemit();
void Iteremit();
void Compoundemit();
void Retemit();
void Callemit();
void Expemit();
void Factemit();
