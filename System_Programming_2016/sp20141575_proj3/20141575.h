#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/* structure that store shell instruction into linked list */
typedef struct node{
	    int index;
		char name[100];
		struct node *link;
}mode_node;       
mode_node* head = NULL; //head node of history linked list


/* structure that store opcode mnemonic and code into hash table */
typedef struct _node{
	    int code;
		char mnemonic[10];
		char inst_form[5];
		struct _node *link;
}hash_table;

hash_table *hashtable[20];	
hash_table *hash_head = NULL;	//head node of hash table linked list

/*structure that store label and LOCCTR into hash table */
typedef struct linknode{
	int LOCCTR;
	char label[10];
	struct linknode *link;
}Sym_Tab;

Sym_Tab *symtab[20];

/* structure that store opcode,opcode mnemonic and instruction form */
typedef struct {
	    int code;
		char mnemonic[10];
		char inst_form[10];
		int hash_num;
}opcode_data;

opcode_data opcode[58];

typedef struct ES{
	char rec_type;
	char name[10];
	int CSADDR;
	int length;
	struct ES *link;
}ES_TAB;

ES_TAB *ESTAB[20];

typedef struct{
	char name[10];
	int ref_num;
}reference;

reference ref[1000];

typedef struct{
	char name[10];
	char rec_type;
	int addr;
	int length;
}map;

map load_map[1000];
unsigned char virt_memory[1048576] = { 0 ,};  //1MByte virtual mememory 

int memory_address = 0;		//memory address that using in mode_dump function
int current_address = 0;	//next memory address that using in mode_dump function

int curr_cnt = 0;
int bp_cnt = 0;
int breakpo[500];
int breakpoint[500];
int sym_addr;
int prog_len = 0;  // program length
int LOCCTR[10000] = { 0 ,};
int T_length[10000] = { 0, };
int BASE = 0;
int run_address;
int sym_address;
/****************************************
	module name  : error_message()
	function     : print error message
	parameter    : Not exist 
*****************************************/
void error_message(); 


/****************************************
	module name  : mode_help()
	function     : printf shell instruction 
	parameter    : Not exist
*****************************************/
void mode_help();


/****************************************
	module name  : mode_quit()
	function     : free memory allocating and quit the program
	parameter    : Not exist
*****************************************/
void mode_quit();


/****************************************
	module name  : mode_dir();
	function     : print the current directory and print '*' beside the execution file and print '/' beside the directory file
	parameter    : Not exist
*****************************************/
void mode_dir();


/****************************************
	module name   : mode_history()
	function      : print index and shell instrution in regular sequence
	parameter     : not exist
*****************************************/
void mode_history();


/****************************************
	module name  : store_history()
	function     : Make linked list that store shell instruction which used
	parameter    : char *mode 
				   int index  
*****************************************/
void store_history(char *mode,int index);


/****************************************
	module name  : mode_dump()
	function     : print memory from start to end
	parameter    : int start 
				  int end
*****************************************/
void mode_dump(int start,int end);


/****************************************
	module name  : mode_reset
	function     : reset virtual memory 
	paraemeter   : Not exist
*****************************************/
void mode_reset();


/****************************************
	module name  : mode_edit
	function     : change memory value to input value at input address
	parameter    : int address
				   int value
*****************************************/
void mode_edit(int address,int value);


/****************************************
	module name  : mode_fill
	function     : fill memory input value from start address to end address 
	parameter    : int start 
				   int end
				   int value
*****************************************/
void mode_fill(int start,int end,int  value);


/****************************************
	module name : mode_opcodelist
	function    : print opcode hash table
	parameter   : not exist
*****************************************/
void mode_opcodelist();


/****************************************
	module name : mode_opcode_find
	function    : find the opcode mnemonic and print opcode
	parameter   : char *opcode_inst
*****************************************/
void mode_opcode_find(char *opcode_inst);


/****************************************
	module name : construct_hashtable
	function    : store opcode structure into hash table structure 
	parameter   : FILE *fp
*****************************************/
void construct_hashtable(FILE *fp);


/****************************************
	module name : make_hash_func
	function    : make hash number by using hash function
	parameter   : char *mnemonic
*****************************************/
int make_hash_func(char *mnemonic);

/****************************************
	module name : Find_Symbol
	function    : match label with symbol table`s label
	parameter   : char *label
****************************************/
int Find_Symbol(char *label);

/****************************************
	module name : construct_Symtab
	function    : construct a symbol table;
	parameter   : int LOCCTR
				  char *label
****************************************/
void construct_Symtab(int LOCCTR,char *label);

/*****************************************
  	module name : Make_Symtab
	function    : make LOCCTR in lst file
	parameter   : FILE *fp
*****************************************/
void Make_Symtab(char *filname);

/****************************************
  	module name : opcode_find
	function    : find opcode in hash function
	parameter   : char *opcode
****************************************/
int opcode_find(char *opcode);

/*****************************************
  	module name : print_symtab
	function    : print a symbol table by descending order
	parameter   : Not exist
******************************************/	
void print_symtab();

/*******************************************
  	module name : Assemble
	function    : make a lst&obj file by assemble
	parameter   : FILE *fp
********************************************/
void Assemble(char *filename);

/********************************************
  	module name : get_opcode
	function 	: get a opcode number
	parameter   : char *opcode
*********************************************/
int get_opcode(char *opcode);

/********************************************
  	module name : format2
	function 	: get a format2 object code
	parameter   : int opcode
				  char reg1
				  char reg2
*********************************************/
int format2(int opcode,char reg1,char reg2);

/********************************************
  	module name : format34
	function 	: get a format 3 or 4 object code
	parameter   : int opcode
			      int nixbpe
				  int disp_addr
*********************************************/
int format34(int opcode,int nixbpe,int disp_addr);

/*********************************************
  	module name : mode_type
	fuction 	: print input file`s string
	parameter   : char *filename
******************************************/
void mode_type(char *filename);

/*******************************************
	module name : free_symtab
	function    : free symbol table
	parameter   : Not exist
******************************************/
void free_symtab();

/******************************************
	module name : mode_progaddr
	function    : set run starting address
	parameter   : int prog_address
******************************************/
void mode_progaddr(int prog_address);

/*******************************************
  	module name : mode_loader
	function    : loading the program
	parameter   : char* proga
				  char* progb
				  char* progc
*********************************************/
void mode_loader(char* proga,char* progb,char* progc);

/*******************************************
  	module name : construct_ESTAB
	function    : make ESTAB 
	parameter   : int CSADDR
				  char *name
				  int length
				  char rec_type
*********************************************/
void construct_ESTAB(int CSADDR,char *name,int length,char rec_type);

/*******************************************
  	module name : search_ESTAB
	function    : search symbol name and if symbol name is in ESTAB return 1; else -1;
	parameter   : char *name
*********************************************/
int search_ESTAB(char *name);




