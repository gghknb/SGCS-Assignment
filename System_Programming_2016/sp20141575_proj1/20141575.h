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
		struct _node *link;
}hash_table;

hash_table *hashtable[20];	
hash_table *hash_head = NULL;	//head node of hash table linked list

/* structure that store opcode,opcode mnemonic and instruction form */
typedef struct {
	    int code;
		char mnemonic[10];
		char inst_form[10];
		int hash_num;
}opcode_data;

opcode_data opcode[58];

unsigned char virt_memory[1048576] = { 0 ,};  //1MByte virtual mememory 

int memory_address = 0;		//memory address that using in mode_dump function
int current_address = 0;	//next memory address that using in mode_dump function


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
	function     : free merry allocating and quit the program
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
	variable    : not exist
*****************************************/
void mode_opcodelist();


/****************************************
	module name : mode_opcode_find
	function    : find the opcode mnemonic and print opcode
	variable    : char *opcode_inst
*****************************************/
void mode_opcode_find(char *opcode_inst);


/****************************************
	module name : construct_hashtable
	function    : store opcode structure into hash table structure 
	variable    : FILE *fp
*****************************************/
void construct_hashtable(FILE *fp);


/****************************************
	module name : make_hash_func
	function    : make hash number by using hash function
	variable    : char *mnemonic
*****************************************/
int make_hash_func(char *mnemonic);
