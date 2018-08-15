#include "20141575.h"

int main()
{	
	char mode[80];
	int index = 0;
	char dump[10],edit[10],fill[10],opcode[10],filename[20];
	char comma,comma2,comma3;
	int start ,end;
	int result,result2;
	int address;
	int prog_address;
	int value;
	char opcode_inst[100];
	FILE *fp;
	char str[80];
	int breakp;
	int program_cnt = 0;
	char proga[20] = {'\0',},progb[20] = {'\0',},progc[20] = {'\0',};
	char breakpo[10];
	int i;
	/* open opcode txt and make hash code table*/
	fp = fopen("opcode.txt","r");
	construct_hashtable(fp);
	fclose(fp);
	run_address = 0; //set run address 0X00
	while(1)
	{
		printf("sicsim> ");
		gets(str);
		/*if string length > 80 print error */
		if(strlen(str) > 80)
		{
			printf("Input error!!\n");
			continue;
		}
		program_cnt++;
		result = sscanf(str,"%s %c",mode,&comma);

		if((strcmp(mode,"help") == 0 || strcmp(mode,"h") == 0) && result == 1)  //help instruction
		{
			mode_help();
		}
		else if((strcmp(mode,"dir") == 0 || strcmp(mode,"d") == 0) && result == 1) //dir instruction
		{
			mode_dir();
			printf("\n");
		}
		else if((strcmp(mode,"history") == 0 || strcmp(mode,"hi") == 0) && result == 1)  //histroy instruction
		{
			mode_history();
			printf("%d %s\n",index+1,str);
		}
		else if((strcmp(mode,"quit") == 0 || strcmp(mode,"q") == 0) && result == 1 )  //quit the program
		{
			free_symtab();
			mode_quit();
		}
		else if(strcmp(mode,"reset") == 0 && result == 1) 		//reset the memory 
		{
			mode_reset();
		}
		else if(*mode == 'd' )  //print the memory
		{
			result = sscanf(str,"%s %x %c %x %c",dump,&start,&comma,&end,&comma2); 
			result2 = sscanf(str,"%s %c",dump,&comma3);
			if(strcmp(dump,"dump") != 0 && strcmp(dump,"du") != 0 )  
			{
				error_message();
				continue;
			}
			if((strcmp(dump,"dump") == 0 || strcmp(dump,"du") == 0) && result == 1 && result2 == 1) // Input : dump or du
			{
				start = current_address;	
				memory_address = start - start % 16;
				end = start + 159;						//set end address start+ 159

			}
			else if((strcmp(dump,"dump") == 0 || strcmp(dump,"du") == 0) && result == 2) // Input : dump start
			{
				if(start < 0 || start >= 1048576)        //if address is out of bound of memory
				{
					error_message();
					continue;
				}
				memory_address = start - start % 16 ;
				end = start + 159;
			}
			else if((strcmp(dump,"dump") == 0 || strcmp(dump,"du") == 0) && comma == ',' && result == 4) // Input : dump start,end
			{	
				if(start > end || (start < 0 || start >= 1048576) || (end < 0 || end >= 1048576) )  // if start and end address is out of bound of memory
				{
					error_message();
					continue;
				}

				memory_address = start - start % 16;
			}
			else 
			{
				error_message();
				continue;
			}

			mode_dump(start,end);
		}
		else if (*mode == 'e' )		//edit the memory 
		{
			result = sscanf(str,"%s %X %c %X %c", edit,&address,&comma,&value,&comma2 );
			if(strcmp(edit,"edit") != 0 && strcmp(edit,"e") != 0)
			{
				error_message();
				continue;
			}

			if( (strcmp(edit,"e") == 0 || strcmp(edit,"edit") == 0) && result == 4 && comma == ',')
			{
				if( (address < 0 || address >= 1048576) || ((int)value < 0 || (int)value > 255) )  //if address is out of bound of memory and value`s ASCII code is out of bound
				{
					error_message();
					continue;
				}

				mode_edit(address,value);

			}
			else
			{
				error_message();
				continue;
			}

		}
		else if (*mode == 'f' )		//fill the memory from start address to end address 
		{
			result = sscanf(str,"%s %X %c %X %c %X %c",fill,&start,&comma,&end,&comma2,&value,&comma3);
			if(strcmp(fill,"fill") != 0 && strcmp(fill,"f") != 0)
			{
				error_message();
				continue;
			}
			else if(result == 6 && comma == ',' && comma2 == ',')
			{
				if( start > end || (start < 0 || start >= 1048576) || (end < 0 || end >= 1048576) || ((int)value < 0 || (int)value > 255)) // start and end address is out of bound and value`s ASCII code is out of bound
				{
					error_message();
					continue;
				}
			}
			else
			{
				error_message();
				continue;
			}
			mode_fill(start,end,value);
		}
		else if(*mode == 'o')
		{
			if(strcmp(mode,"opcodelist") == 0)
			{
				mode_opcodelist();	
			}
			else
			{
				result = sscanf(str,"%s %s %c",opcode,opcode_inst,&comma);
				if(strcmp(opcode,"opcode") == 0 && result == 2)  //Input : opcode mnemonic
				{
					mode_opcode_find(opcode_inst);
				}
				else
				{

					error_message();
					continue;
				}
			}
		}
		else if(strcmp(mode,"assemble") == 0)
		{
			result = sscanf(str,"%s %s %c",mode,filename,&comma);
			if(result == 2)
			{
				if(program_cnt >= 2)
					free_symtab();

				Make_Symtab(filename);
				Assemble(filename);
			}
			else
			{
				error_message();
				continue;
			}
		}
		else if(strcmp(mode,"symbol") == 0)
		{
			if(result == 1)
			{
				print_symtab();
			}
			else
			{
				error_message();
				continue;
			}
		}
		else if(strcmp(mode,"type") == 0)
		{
			result = sscanf(str,"%s %s %c",mode,filename,&comma);
			if(result == 2)
			{
				mode_type(filename);
			}
			else 
			{
				error_message();
				continue;
			}
		}
		else if(strcmp(mode,"progaddr") == 0)
		{
			result = sscanf(str,"%s %X %c",mode,&prog_address,&comma);
			if(result == 2)
			{
				mode_progaddr(prog_address);
			}
		}
		else if(strcmp(mode,"loader") == 0)
		{
			result = sscanf(str,"%s %s %s %s %c",mode,proga,progb,progc,&comma);
			if(result >= 2 && result <= 4)
			{	
				mode_loader(proga,progb,progc);
				bp_cnt = 0;
				curr_cnt = 0;
			}
		}
		else if(strcmp(mode,"bp") == 0)
		{
			memset(breakpo,'\0',500);
			result = sscanf(str,"%s %s %c",mode,breakpo,&comma);
			if(result ==1)
			{
				if(bp_cnt != 0)
				{
					printf("	breakpoint\n");
					printf("	----------\n");
					for(i = 0 ; i < bp_cnt ; i++)
						printf("	%X\n",breakpoint[i]);
				}
				else 
					printf("	There is no breakpoint\n");
			}
			else if(result == 2)
			{
				if(strcmp(breakpo,"clear") == 0)
				{
					memset(breakpoint,0,500);
					bp_cnt = 0;
					curr_cnt = 0;
					printf("	[ok] clear all breakpoints\n");
				}
				else if((breakpo[0] >= '0' && breakpo[0] <= '9') || (breakpo[0] >= 'A' && breakpo[0] <= 'F'))
				{
					result = sscanf(str,"%*s %x %c",&breakp,&comma);
					if(result == 1)
					{
						breakpoint[bp_cnt] = breakp;
						bp_cnt++;
					}
					else
					{
						printf("error!!\n");
						continue;
					}
				}
				else
				{
					printf("Error!\n");
					continue;
				}	
			}
			else
			{
				printf("error!!\n");
				continue;
			}
		}
		else 
		{	
			error_message();
			continue;
		}

		index++;
		store_history(str,index);
	}
}

void mode_help()
{
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start,end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
	printf("progaddr address\n");
	printf("loader filename\n");
	printf("run\n");
	printf("bp address\n");
	return ;
}

void mode_dir()
{
	DIR *dirp;
	struct dirent *direntp;
	struct stat buf;
	dirp = opendir(".");

	while((direntp = readdir(dirp)) != NULL)
	{
		stat(direntp->d_name,&buf);
		if(strcmp(direntp->d_name,".") ==  0 || strcmp(direntp->d_name,"..") == 0 )  //remove '.' and '..' file
			return ;

		if(S_ISDIR(buf.st_mode))		//if directory file print '/'
		{
			printf("%s/ \t",direntp->d_name);
		}
		else if(S_IXUSR & buf.st_mode)	//if execution file print '*'
		{
			printf("%s* \t",direntp->d_name);
		}
		else printf("%s \t",direntp->d_name);
	}

	closedir(dirp);
}

void mode_quit()
{
	hash_table *temp;
	mode_node *tmp;
	int i;
	/* free the dynamic allocated*/
	if(!head)
	{
		while(head != NULL)
		{
			tmp = head;
			head = head->link;
			free(tmp);
		}
	}

	for(i = 0 ; i < 20 ; i++)
	{
		while(hashtable[i] != NULL)
		{
			temp = hashtable[i];
			hashtable[i] = hashtable[i]->link;
			free(temp);
		}
	}

	exit(-1);
}

void mode_history()
{
	mode_node *print_node;
	/*print the linked list*/
	for(print_node = head ; print_node ; print_node = print_node->link)
	{
		printf("%d %s\n",print_node->index,print_node->name);
	}
}

void store_history(char *mode,int index)
{
	mode_node *new1,*new2;
	new1 = (mode_node*)malloc(sizeof(mode_node));
	strcpy(new1->name,mode);
	new1->link = NULL;
	if(!head) 		//Make linked list if list is empty
	{
		new1->index = 1;
		head = new1;
	}
	else			//If list is not empty , add the list 
	{	
		for(new2 = head ; new2->link ;new2 = new2->link)
			;
		new1->index = index;
		new2->link = new1;
	}
}

void mode_dump(int start,int end)
{
	int i,j;

	/*if end memory >= 1048576,print memory from start memory to end address of memory(1048575) */
	if(end>=1048576)
		end=1048575;


	for(i = (start / 16 ) ; i < (end / 16) + 1 ; i++)
	{
		printf("%05X ",memory_address);

		if( start / 16 == end / 16) //if start and end are same moemry line 
		{
			for(j = memory_address ; j < memory_address + start % 16 ; j++)
				printf("   ");
			for(j = memory_address + start % 16 ; j <= memory_address + end % 16 ; j++)
				printf("%02X ",virt_memory[j]);
			for(j = memory_address + end % 16 + 1 ; j <  memory_address + 16 ; j++)
				printf("   ");

			printf("; ");
			for(j = memory_address ; j < memory_address + 16; j++)
			{
				if((int)virt_memory[j] < 32 || (int)virt_memory[j] > 126)
					printf(".");
				else
					printf("%c",virt_memory[j]);
			}
			printf("\n");
		}
		else if( i == start / 16 && start != 0 ) //print first memory line
		{
			for(j = memory_address ; j < memory_address + start % 16; j++)
				printf("   ");

			for(j = memory_address + start % 16 ; j < memory_address + 16 ; j++)	
				printf("%02X ",virt_memory[j]);

			printf("; ");
			for(j = memory_address ; j< memory_address + start % 16 ; j++)
				printf(".");
			for(j = memory_address + start % 16 ; j < memory_address + 16; j++)
			{
				if((int)virt_memory[j] < 32 || (int)virt_memory[j] > 126 )
					printf(".");
				else 
					printf("%c",virt_memory[j]);
			}

			printf("\n");
		}
		else if(i == end / 16)	//print last memory line
		{
			for(j = memory_address ; j <= memory_address + end % 16 ; j++)
				printf("%02X ",virt_memory[j]);

			for(j = memory_address + end % 16 + 1   ; j < memory_address + 16 ; j++)
				printf("   ");

			printf("; ");
			for(j = memory_address ; j <= memory_address + end % 16 ; j++)
			{
				if((int)virt_memory[j] < 32 || (int)virt_memory[j] > 126)
					printf(".");
				else	
					printf("%c",virt_memory[j]);
			}

			for(j = memory_address + end % 16 + 1; j < memory_address + 16 ; j++)
				printf(".");

			printf("\n");
		}
		else 			//print memory line except last and first memory line
		{
			for(j = memory_address ; j < memory_address + 16 ; j++)
				printf("%02X ",virt_memory[j]);

			printf("; ");

			for(j = memory_address ; j < memory_address + 16 ; j++)
			{
				if((int)virt_memory[j] < 32 || (int)virt_memory[j] > 126)
					printf(".");
				else	
					printf("%c",virt_memory[j]);
			}
			printf("\n");
		}

		memory_address += 16;
	}
	if(end == 1048575)
		current_address = 0;
	else
		current_address = end + 1;
}

void mode_reset()
{
	int i;
	for(i = 0 ; i < 1048576; i++)
		virt_memory[i] = 0 ;
}

void mode_edit(int address,int value)
{
	virt_memory[address] = value;
}

void mode_fill(int start,int end,int value)
{
	int i;
	for(i = start ; i <= end ; i++)
		virt_memory[i] = value;
}

void error_message()
{
	printf("error!!\n");
	return ;
}

void mode_opcodelist()
{
	int i;
	hash_table *print_node;
	/*print the hash opcode table */
	for(i = 0 ; i < 20 ; i++)
	{
		printf("%d : ",i);
		for(print_node = hashtable[i]->link ; print_node ; print_node = print_node->link)
		{
			printf("[%s,%02X]  ",print_node->mnemonic,print_node->code);
			if(print_node->link)
				printf("-> ");
		}
		printf("\n");
	}

}

void mode_opcode_find(char *opcode_inst)
{
	int flag = 0;
	int hash_num;
	hash_table *temp;
	/* find opcode mnemonic by sung hash number */	
	hash_num = make_hash_func(opcode_inst);
	for(temp = hashtable[hash_num] ; temp != NULL ; temp = temp->link)
	{
		if(strcmp(opcode_inst,temp->mnemonic) == 0 )
		{
			printf("opcode is %X\n",temp->code);
			flag = 1;
			break;
		}
	}
	if(flag == 0)
		printf("%s is not a opcode mnemonic\n",opcode_inst);
}

void construct_hashtable(FILE *fp)
{
	int i,j;
	char str[100];
	hash_table *new1,*new2;
	/* store the information into opcode structure */
	for(i = 0; i < 58 ; i++)
	{
		fgets(str,100,fp);
		sscanf(str,"%X %s %s",&opcode[i].code,opcode[i].mnemonic,opcode[i].inst_form);
		opcode[i].hash_num  = make_hash_func(opcode[i].mnemonic);
	}

	/*Make a hash table */
	for(i = 0 ; i < 20 ; i++)
	{
		hashtable[i] = (hash_table*)malloc(sizeof(hash_table));
		hash_head = hashtable[i];
		for(j = 0 ; j < 58 ; j++)
		{
			if(i == opcode[j].hash_num)
			{
				new1 = (hash_table*)malloc(sizeof(hash_table));
				strcpy(new1->mnemonic,opcode[j].mnemonic);
				strcpy(new1->inst_form,opcode[j].inst_form);
				new1->code = opcode[j].code;
				new1->link = NULL;

				if(!hash_head)
					hash_head = new1;
				else
				{
					for(new2 = hash_head ; new2->link; new2 = new2->link)
						;
					new2->link = new1;
				}
			}
		}
	}
}

int make_hash_func(char *mnemonic)
{
	int i,len;
	int sum = 0;
	len = strlen(mnemonic);
	for(i = 0 ; i < len ; i++)
		sum += mnemonic[i];
	return sum % 20;
}

void Make_Symtab(char *filename)
{
	FILE *fp;
	char str[100];							//input string
	char temp1[10],temp2[10],temp3[40],temp4[40],temp5[40];		//input of asm file
	int address,startingAddress;		//input address, starting address to get length 
	int ctrcnt = 0;			//location counter index,
	char label[10],opcode[10],symbol[40];  //input
	int LOCCTRtmp;  //location counter value 
	int idx = 0;
	fp = fopen(filename,"r");
	if(fp == NULL)		//if filename is not exist, print error
	{
		printf("file does not exist\n");
		return ;
	}

	fgets(str,100,fp);		//get first line
	sscanf(str,"%s %s %d",label,opcode,&address);

	while(1)		//First input line
	{
		if(strcmp(opcode,"START") == 0 )
		{
			startingAddress = address; //save starting address
			LOCCTRtmp = address;	//initialize LOCCTR to starting address
			break;
		}
		else if(*label  == '.' )  //if comment line
		{
			fgets(str,100,fp);
			sscanf(str,"%s %s %d",label,opcode,&address);
		}
		else return ;
	}
	LOCCTR[ctrcnt] = LOCCTR[ctrcnt+1] = LOCCTRtmp;
	/* FROM FIRST LINE TO LAST LINE */
	while(1)
	{
		/*read next line & initailize variable */
		memset(temp1,'\0',strlen(temp1));
		memset(temp2,'\0',strlen(temp2));
		memset(temp3,'\0',strlen(temp3));
		memset(temp4,'\0',strlen(temp4));
		memset(temp5,'\0',strlen(temp5));
		memset(opcode,'\0',strlen(opcode));
		memset(label,'\0',strlen(label));
		memset(symbol,'\0',strlen(symbol));
		fgets(str,100,fp);
		sscanf(str,"%s %s %s %s %s",temp1,temp2,temp3,temp4,temp5);	
		if(*temp1 == '.')
			continue;
		else if(strcmp(temp1,"END") == 0)
			break;
		else if(opcode_find(temp1) != 0 )
		{
			strcpy(opcode,temp1);
			if(temp4 != NULL && strcmp(temp3,",") == 0 )
			{
				strcat(temp2,temp3);
				strcat(temp2,temp4);
			}

			strcpy(symbol,temp2);
		}
		else if(opcode_find(temp2) != 0)
		{
			strcpy(opcode,temp2);
			strcpy(label,temp1);
			if(strcmp(temp4,",") == 0)
			{
				strcat(temp3,temp4);
				strcat(temp4,temp5);
			}

			strcpy(symbol,temp3);
		}
		else if(strcmp(temp2,"RESW") == 0 || strcmp(temp2,"RESB") == 0 ||strcmp(temp2,"WORD") == 0 ||strcmp(temp2,"BYTE") == 0)  //if opcode is RESW or RESB, get number of WORD or BYTE
		{
			strcpy(label,temp1);
			strcpy(opcode,temp2);
			strcpy(symbol,temp3);

			if(strcmp(opcode,"WORD") == 0 )  //if opcode is WORD , range of number is less than FFFFFF
			{
				if(atoi(symbol) < 0 || atoi(symbol) > 16777216)
				{
					continue;
				}		
			}
		}
		else if(strcmp(temp1,"BASE") == 0)
		{
			strcpy(opcode,temp1);
			strcpy(symbol,temp2);
		}
		else 
		{
			continue;
		}
		ctrcnt++;
		LOCCTR[ctrcnt] = LOCCTRtmp;

		if(*label != '.')
		{
			if(strlen(label) != 0 )  //find symbol in label field
			{
				if(Find_Symbol(label) !=  -1 ) //search Symtab for LABEL
				{
					printf("duplicate symbol\n");
					continue;
				}
				else
				{
					construct_Symtab(LOCCTRtmp,label);
				}
			}
			/* ADD LOCCTR */
			if(opcode_find(opcode) != 0)	//add 3 (instruction length) to LOCCTR
			{
				LOCCTRtmp += opcode_find(opcode);	
				if(T_length[idx] + opcode_find(opcode) > 30)
				{
					T_length[idx+1] += opcode_find(opcode);
					idx++;
				}
				else
					T_length[idx] += opcode_find(opcode);

			}
			else if(strcmp(opcode,"WORD") == 0)  //add 3 to LOCCTR
			{
				LOCCTRtmp += 3;
				if(T_length[idx] + 3 > 30)
				{
					T_length[idx+1] += 3;
					idx++;
				}
				else 
					T_length[idx] += 3;
			}
			else if(strcmp(opcode,"RESW") == 0)  //add 3* #[OPERAND] to LOCCTR
			{
				LOCCTRtmp += 3*atoi(symbol);
				if(T_length[idx]  != 0)
					idx++;
			}
			else if(strcmp(opcode,"RESB") == 0) //add #[OPERAND] to LOCCTR
			{
				LOCCTRtmp += atoi(symbol);
				if(T_length[idx] != 0)
					idx++;

			}
			else if(strcmp(opcode,"BYTE") == 0)  //find length of constant in bytes and add length to LOCCTR
			{
				if(symbol[0] == 'C')  
				{
					LOCCTRtmp += strlen(symbol) - 3;
					if(T_length[idx] + strlen(symbol) -3 > 30)
					{
						T_length[idx+1] += strlen(symbol) -3;
						idx++;
					}
					else
						T_length[idx] += strlen(symbol) - 3;
				}
				else if(symbol[0] ==  'X')
				{
					if(strlen(symbol) % 2 == 0)
					{
						continue;
					}
					LOCCTRtmp += (strlen(symbol) - 3) / 2;
					if(T_length[idx] +( strlen(symbol) -3)/2 > 30)
					{
						T_length[idx+1] += (strlen(symbol) -3)/2;
						idx++;
					}
					else
						T_length[idx] += (strlen(symbol) - 3)/2;	
				}
			}
			else if(strcmp(opcode,"BASE") == 0)
				;	
			else
			{
				continue;
			}
		}
	}


	prog_len = LOCCTRtmp - startingAddress;  //save as program length
	fclose(fp);	
}

int Find_Symbol(char *label)
{
	int hash_num;
	Sym_Tab *new1;
	hash_num = make_hash_func(label);
	for(new1 = symtab[hash_num] ; new1 != NULL ; new1 = new1->link)
	{

		if(strcmp(new1->label,label) == 0 )
		{
			return new1->LOCCTR;
		}
	}
	return -1;
}

void construct_Symtab(int LOCCTR,char *label)
{	
	int hash_num;
	Sym_Tab *new1,*new2;

	new1 = (Sym_Tab*)malloc(sizeof(Sym_Tab));
	hash_num = make_hash_func(label);
	new1->LOCCTR = LOCCTR;
	strcpy(new1->label,label);
	new1->link = NULL;   
	if(!symtab[hash_num])    //if symtable[hash_num] is empty
		symtab[hash_num] = new1;
	else					
	{
		for(new2 = symtab[hash_num] ; new2->link ; new2 = new2->link)
			;
		new2->link = new1;
	}
}

int opcode_find(char *opcode)
{
	int hash_num;
	hash_table *temp;
	char new_opcode[10];
	int inst_form;
	if(opcode[0] == '+')
	{
		strcpy(new_opcode,opcode+1);
	}
	else strcpy(new_opcode,opcode);

	/* find opcode mnemonic by hash number */	
	hash_num = make_hash_func(new_opcode);
	for(temp = hashtable[hash_num] ; temp != NULL; temp = temp->link)
	{
		if(strcmp(new_opcode,temp->mnemonic) == 0 )
		{
			if(strcmp(temp->inst_form,"1") == 0)
				inst_form = 1;
			else if(strcmp(temp->inst_form,"2") == 0)
				inst_form = 2;
			else if(opcode[0] == '+')
				inst_form = 4;
			else 
				inst_form = 3;

			return inst_form;  //return instruction form of opcode

		}
	}
	return 0;
}


void free_symtab()
{
	Sym_Tab *temp;
	int i;
	for(i = 0 ; i < 10000 ; i++) //initialize global variable
	{
		LOCCTR[i] = 0;
		T_length[i] = 0;
	}

	for(i = 0 ; i < 20 ; i++)   //free symboltable
	{
		while(symtab[i] != NULL)
		{
			temp = symtab[i];
			symtab[i] = symtab[i]->link;
			free(temp);
		}
	}

}

void print_symtab(void)
{
	int i,j=0,cnt=0;
	Sym_Tab *new1;
	Sym_Tab temp[100];
	char tmp[10];
	int temp1;
	for(i = 0 ; i < 20 ; i++)
	{
		for(new1 = symtab[i] ; new1 != NULL ; new1 = new1-> link)
		{
			temp[j].LOCCTR = new1->LOCCTR;
			strcpy(temp[j].label,new1->label);
			j++;
			cnt++;
		}
	}
	for(i = 0 ; i < cnt  ; i++)  //selection sort about Symbol label
	{
		for(j = i + 1; j < cnt  ; j++)
		{
			if(strcmp(temp[i].label,temp[j].label) < 0 )
			{
				strcpy(tmp,temp[i].label);
				strcpy(temp[i].label,temp[j].label);
				strcpy(temp[j].label,tmp);
				temp1 = temp[i].LOCCTR;
				temp[i].LOCCTR = temp[j].LOCCTR;
				temp[j].LOCCTR = temp1;

			}
		}
	}
	for(i = 0 ; i < cnt ; i++)
	{
		printf("\t%s\t%04X\n",temp[i].label,temp[i].LOCCTR);
	}

}

void Assemble(char *filename)
{
	char str[100];  //input string 
	char objname[20],lstname[20];  //name of obj,lst file
	char *nametmp;
	FILE *fp1,*fp2,*fp3;
	int address;
	char label[10],opcode[10],symbol[40];
	char temp1[10],temp2[10],temp3[40],temp4[40],temp5[40];  //input word
	int result;			//result of sscanf
	int line = 0;
	int inst_form;		// instruction form
	int obj_length = 0;  //length of 1 line object code
	int PC,TA;			//program counter, Target Address
	int base_flag = 0 ;  //if 0 then pc addressing else if 1 then base addressing
	int objcode;   //object code
	int ctrcnt = 0 ;		//location counter index
	int nixbpe;  //nixbpe bit
	int flag,n_flag=0,i_flag=0,x_flag=0 ,b_flag=0,p_flag=0 ,e_flag=0 ;   //to make nixbpe bit
	char strreg[30];	//register name
	int i,num,idx =0,cnt=0;			
	char error,error1,error2;  //to check error of input
	char str_temp[30];  //string of C or X(BYTE)
	int displacement;  //gap of Target address and PC,BASE address
	char filenametemp[20];
	int modification[100] = {0,};
	/* make .lst & .obj file */
	strcpy(filenametemp,filename);
	nametmp = strtok(filenametemp,".");
	strcpy(lstname,nametmp);
	strcpy(objname,nametmp);
	strcat(objname,".obj");
	strcat(lstname,".lst");
	/* open files to read and write */
	fp1 = fopen(filename,"r");
	if(fp1 == NULL)
	{
		return ;
	}
	fp2 = fopen(lstname,"w");
	fp3 = fopen(objname,"w");
		

	/*get line*/
	fgets(str,100,fp1);
	result = sscanf(str,"%s %s %d",label,opcode,&address);
	strtok(str,"\n");
	
	/* Write First line */
	while(1)
	{	
		line += 5;
		if(result == 3 && strcmp(opcode,"START") == 0 )
		{
			fprintf(fp2,"%3d\t%04X\t%s\n",line,address,str);  //write to .lst file 
			fprintf(fp3,"H%-6s%06X%06X",label,address,prog_len);	//write to .obj file
			ctrcnt++;
			break;
		}
		else if(*label  == '.' )  //if comment line
		{
			fgets(str,100,fp1);
			result = sscanf(str,"%s %s %d",label,opcode,&address);
			strtok(str,"\n");
			ctrcnt++;
		}
		else 
		{
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			printf("error!!\n");
			return ;
		}
	}

	/* write to second line to last line */
	while(1)
	{

		/* initialize variable */
		memset(temp1,'\0',strlen(temp1));
		memset(temp2,'\0',strlen(temp2));
		memset(temp3,'\0',strlen(temp3));
		memset(temp4,'\0',strlen(temp4));
		memset(temp5,'\0',strlen(temp5));
		memset(opcode,'\0',strlen(opcode));
		memset(label,'\0',strlen(label));
		memset(symbol,'\0',strlen(symbol));	
		memset(strreg,'\0',strlen(strreg));
		memset(str_temp,'\0',strlen(str_temp));
		
		objcode = 0;
		n_flag = 0;
		i_flag = 0;
		x_flag = 0;
		b_flag = 0;
		p_flag = 0;
		e_flag = 0;   //to make nixbpe bit

		/* read line */
		fgets(str,100,fp1);
		result = sscanf(str,"%s %s %s %s %s",temp1,temp2,temp3,temp4,temp5);
		strtok(str,"\n");	
		/* if END line ,write to file */
		if(strcmp(temp1,"END") == 0 )
		{
			line += 5;
			fprintf(fp2,"%3d\t\t%s\n",line,str);
			break;
		}
		else if(*temp1 == '.')
			continue;

		line += 5;
		PC = LOCCTR[ctrcnt+1];
		if(Find_Symbol(temp1) == -1 && opcode_find(temp1) == 4) //label not exist,  temp1 = opcode temp2 = symbol format4
		{
			inst_form = 4;
			e_flag = 1;
			if(Find_Symbol(temp2) != -1 && temp3[0] == '\0')  //symbol exist
			{
				TA = Find_Symbol(temp2);
				if(TA != -1)
				{	
					modification[cnt] = LOCCTR[ctrcnt] -address + 1;
					cnt++;
					n_flag = 1;
					i_flag = 1;
					nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
					objcode = format34(get_opcode(temp1),nixbpe,TA);
					fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);	
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);
						obj_length = 0;
						idx++;
					}
					fprintf(fp3,"%08X",objcode);
					obj_length += inst_form;
				}
				else
				{
					printf("Invalid operation code : Line %d\n",line);
					ctrcnt++;
					flag =1;
					continue;
				}
			}
			else if(*temp2 == '@') // if first letter of operand is @
			{
				TA = Find_Symbol(temp2+1);

				if(TA != -1)
				{
					modification[cnt] = LOCCTR[ctrcnt] -address + 1;
					cnt++;
					n_flag = 1;
					i_flag = 0;
					nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
					objcode = format34(get_opcode(temp1),nixbpe,TA);	
					fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);	
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);
						obj_length = 0;
						idx++;
					}
					fprintf(fp3,"%08X",objcode);
					obj_length += inst_form;
				}
				else 
				{
					
					printf("Invalid operation code : Line %d\n",line);
					ctrcnt++;
					flag =1;
					continue;
				}
			}
			else if(*temp2 == '#') // if first letter of operand is #
			{
				i_flag = 1;
				TA = Find_Symbol(temp2+1);  
				if(TA != -1)   //operand is Symbol
				{
					modification[cnt] = LOCCTR[ctrcnt] -address + 1;
					cnt++;
					nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
					objcode = format34(get_opcode(temp1),nixbpe,TA);
					fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%08X",objcode);	
					obj_length += inst_form;
				}
				else  		//operand is num
				{
					sscanf(temp2+1,"%d",&displacement);
					if(displacement >= 0 && displacement <=65535)
					{
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%08X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1;
						continue;
					}

				}
			}
			else  /*Buffer,X */
			{
				modification[cnt] = LOCCTR[ctrcnt] -address + 1;
				cnt++;

				n_flag = 1;
				i_flag = 1;
				x_flag = 1;
				/* initailize string  */
				strcpy(strreg,temp2);
				strcat(strreg,temp3);
				strcat(strreg,temp4);
				memset(temp2,'\0',strlen(temp2));
				memset(temp3,'\0',strlen(temp3));
				memset(temp4,'\0',strlen(temp4));
				for(i = 0 ; i < strlen(strreg) ; i++)
				{		
					if(strreg[i] == ',')
						break;
					temp2[i] = strreg[i];	
				}
				temp3[0] = strreg[i];
				strcpy(temp4,strreg+i+1);
				inst_form = opcode_find(temp1);

				if(strcmp(temp3,",") == 0 && strcmp(temp4,"X") == 0)
				{
					x_flag = 1;
					n_flag = 1;
					i_flag = 1;
					TA = Find_Symbol(temp2);	
					if(TA != -1 && inst_form ==3 )
					{
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,TA);
						fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);	
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%08X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag =1;
						continue;
					}

				}
			}

			ctrcnt++;
		}
		else if(Find_Symbol(temp1) == -1 && opcode_find(temp1) != 0 && opcode_find(temp1) != 4) //label not exist, temp1 = opcode temp2 = symbol format1,2,3
		{
			inst_form = opcode_find(temp1);

			if(Find_Symbol(temp2) == -1 && temp2[0] == '\0') // if symbol does not exist
			{
				if(inst_form == 1)
				{
					objcode = get_opcode(temp1); 
					fprintf(fp2,"%3d\t%04X\t%s\t\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%06X",objcode);	
					obj_length += inst_form;
				}
				else if(inst_form == 2)
				{
					objcode = format2(get_opcode(temp1),0,0);
					fprintf(fp2,"%3d\t%04X\t%s\t\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%06X",objcode);	
					obj_length += inst_form;
				}
				else if (inst_form ==3)   
				{
					n_flag = 1;
					i_flag = 1;
					nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
					objcode = format34(get_opcode(temp1),nixbpe,0);
					fprintf(fp2,"%3d\t%04X\t%s\t\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%06X",objcode);	
					obj_length += inst_form;
				}
				else
				{
					printf("Invalid operation code : Line %d\n",line);
					ctrcnt++;
					flag = 1; 
					continue;
				}
			}
			else if(Find_Symbol(temp2) != -1 && *temp3 =='\0')  //if symbol exist (simple addressing) n = 1 i = 1
			{

				n_flag = 1;
				i_flag = 1;
				TA = Find_Symbol(temp2);
				inst_form = opcode_find(temp1);
				if(inst_form == 3)
				{
					if( TA - PC >= -2048 && TA - PC <= 2047) // if pc addressing
					{
						p_flag = 1;
						displacement = TA - PC;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=1 i=1 x=0 b=0 p=1 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else if(base_flag == 1 && (TA - BASE >= 0 && TA - BASE <= 4095)) //if base addressing
					{
						b_flag = 1;
						displacement = TA - BASE;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=1 i=1 x=0 b=1 p=0 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else 
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1; 
						continue;
					}
				}
			}
			else if(*temp2 == '#') //symbol has '#' (immediate addressing) n = 0 i = 1
			{
				i_flag = 1;
				inst_form = opcode_find(temp1);
				if(Find_Symbol(temp2 + 1) != -1 ) // #+symbol
				{
					TA = Find_Symbol(temp2 + 1);

					if(TA - PC >= -2048 && TA - PC <= 2047)  //PC addressing
					{
						p_flag = 1;
						displacement = TA - PC;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0

						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	

						obj_length += inst_form;
					}
					else if(base_flag == 1 &&( TA - BASE >= 0 && TA - BASE <= 4095 ))  //BASE addressing
					{
						b_flag = 1;
						displacement = TA - BASE;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=1 p=0 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else  
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1; 
						continue;
					}
				}
				else  //# + num 
				{
					sscanf(temp2+1,"%d",&displacement);
					if(displacement >= 0 && displacement <=65535)
					{
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1;
						continue;
					}
				}	
			}
			else if(*temp2 == '@') //symbol has '@' (indirect addressing[pointer]) n = 1 i = 0
			{
				n_flag = 1;
				inst_form = opcode_find(temp1);
				if(Find_Symbol(temp2 + 1) != -1) // #+symbol
				{
					TA = Find_Symbol(temp2 + 1);
					if(TA - PC >= -2048 && TA - PC <= 2047)  //PC addressing
					{
						p_flag = 1;
						displacement = TA - PC;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else if(base_flag == 1 &&(TA - BASE >= 0 && TA - BASE <= 4095 ))  //BASE addressing
					{
						b_flag = 1;
						displacement = TA - BASE;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp1),nixbpe,displacement);			// n=0 i=1 x=0 b=1 p=0 e=0
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;				
					}
					else  
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1; 
						continue;
					}
				}
			}
			else // 2 register or buffer,X
			{
				/* 1 register */
				inst_form = opcode_find(temp1);

				if(strcmp(temp2,"A") == 0 || strcmp(temp2,"X") == 0 || strcmp(temp2,"B") == 0 || strcmp(temp2,"S") == 0 || strcmp(temp2,"T") == 0 ||  strcmp(temp2,"F") == 0 || strcmp(temp2,"L") == 0)
				{
					if(opcode_find(temp1) == 2)
					{
						objcode = format2(get_opcode(temp1),temp2[0],0);
						fprintf(fp2,"%3d\t%04X\t%s\t\t%04X\n",line,LOCCTR[ctrcnt],str,objcode);	
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%04X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag =1;
						continue;
					}
				}
				else  /* 2 register or buffer,X*/
				{
					/* initailize string  */
					strcpy(strreg,temp2);
					strcat(strreg,temp3);
					strcat(strreg,temp4);
					memset(temp2,'\0',strlen(temp2));
					memset(temp3,'\0',strlen(temp3));
					memset(temp4,'\0',strlen(temp4));
					for(i = 0 ; i < strlen(strreg) ; i++)
					{		
						if(strreg[i] == ',')
							break;
						temp2[i] = strreg[i];	
					}
					temp3[0] = strreg[i];
					strcpy(temp4,strreg+i+1);
					inst_form =opcode_find(temp1);
					/* 2 register */
					if(inst_form == 2 && strcmp(temp3,",") == 0)
					{
						if(strcmp(temp2,"A") == 0 || strcmp(temp2,"X") == 0 || strcmp(temp2,"B") == 0 || strcmp(temp2,"S") == 0 || strcmp(temp2,"T") == 0 || strcmp(temp2,"F") == 0 || strcmp(temp2,"L") == 0)
						{
							if(strcmp(temp4,"A") == 0 || strcmp(temp4,"X") == 0 || strcmp(temp4,"B") == 0 || strcmp(temp4,"S") == 0 || strcmp(temp4,"T") == 0 || strcmp(temp4,"F") == 0 || strcmp(temp4,"L") == 0)
							{
								objcode = format2(get_opcode(temp1),temp2[0],temp4[0]);
								fprintf(fp2,"%3d\t%04X\t%s\t%04X\n",line,LOCCTR[ctrcnt],str,objcode);
								if(obj_length + inst_form > 30 || obj_length == 0 )
								{
									fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
									obj_length = 0;			
									idx++;
								}
								fprintf(fp3,"%04X",objcode);	
								obj_length += inst_form;

							}
							else
							{
					printf("Invalid operation code : Line %d\n",line);
								ctrcnt++;
								flag =1;
								continue;
							}
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag =1;
							continue;
						}
					}
					/* BUFFER, X*/
					else if(strcmp(temp3,",") == 0 && strcmp(temp4,"X") == 0)
					{
						x_flag = 1;
						n_flag = 1;
						i_flag = 1;
						TA = Find_Symbol(temp2);	
						if(TA != -1 && inst_form ==3 )
						{
							if( TA - PC >= -2048 && TA - PC <= 2047)
							{
								p_flag = 1;
								displacement = TA - PC;
								nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
								objcode = format34(get_opcode(temp1),nixbpe,displacement);
								fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
								if(obj_length + inst_form > 30 || obj_length == 0 )
								{
									fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
									obj_length = 0;			
									idx++;
								}
								fprintf(fp3,"%06X",objcode);	
								obj_length += inst_form;


							}
							else if (TA - BASE >= 0 && TA - BASE <= 4095)
							{
								b_flag = 1;
								displacement = TA - BASE;
								nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
								objcode = format34(get_opcode(temp1),nixbpe,displacement);
								fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
								if(obj_length + inst_form > 30 || obj_length == 0 )
								{
									fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
									obj_length = 0;			
									idx++;
								}
								fprintf(fp3,"%06X",objcode);	
								obj_length += inst_form;


							}
							else
							{
					printf("Invalid operation code : Line %d\n",line);
								ctrcnt++;
								flag =1;
								continue;
							}
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag =1;
							continue;
						}
					}
					else
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag =1;
						continue;
					}

				}
			}
			ctrcnt++;	
		}
		else if(Find_Symbol(temp1) != -1 && opcode_find(temp2) != 0 && opcode_find(temp2) != 4 )  //label exist, temp1 = label, temp2 = opcode format :1,2, 3
		{
			if(Find_Symbol(temp1) != -1)			//label is exist on symbol table
			{
				inst_form = opcode_find(temp2);
				if(Find_Symbol(temp3) == -1 && temp3[0] == '\0') // if symbol does not exist
				{
					if(inst_form == 1)
					{
						objcode = get_opcode(temp2); 
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else if(inst_form == 2)
					{
						objcode = format2(get_opcode(temp2),0,0);
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else if (inst_form ==3)   
					{
						n_flag = 1;
						i_flag = 1;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp2),nixbpe,0);
						fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%06X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag = 1; 
						continue;
					}
				}
				else if(Find_Symbol(temp3) != -1 && *temp4 =='\0')  //if symbol exist (simple addressing) n = 1 i = 1
				{
					n_flag = 1;
					i_flag = 1;
					TA = Find_Symbol(temp3);
					inst_form = opcode_find(temp2);
					if(inst_form == 3 )
					{
						if((TA - PC) >= -2048 && (TA - PC) <= 2047) // if pc addressing
						{
							p_flag = 1;
							displacement = TA - PC;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=1 i=1 x=0 b=0 p=1 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else if(base_flag == 1 && ( TA - BASE >= 0 && TA - BASE <= 4095)) //if base addressing
						{
							b_flag = 1;
							displacement = TA - BASE;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=1 i=1 x=0 b=1 p=0 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else 
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag = 1; 
							continue;
						}
					}
				}
				else if(*temp3 == '#') //symbol has '#' (immediate addressing) n = 0 i = 1
				{
					i_flag = 1;
					inst_form = opcode_find(temp2);
					if(Find_Symbol(temp3 + 1) != -1) // #+symbol
					{
						TA = Find_Symbol(temp3 + 1);
						if(TA - PC >= -2048 && TA - PC <= 2047)  //PC addressing
						{
							p_flag = 1;
							displacement = TA - PC;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else if(base_flag == 1 &&(TA - BASE >= 0 && TA - BASE <= 4095))  //BASE addressing
						{
							b_flag = 1;
							displacement = TA - BASE;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=1 p=0 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else  
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag = 1; 
							continue;
						}
					}
					else  //# + num 
					{
						sscanf(temp3+1,"%d",&displacement);
						if(displacement >= 0 && displacement <=65535)
						{
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag = 1;
							continue;
						}
					}	
				}
				else if(*temp3 == '@') //symbol has '@' (indirect addressing[pointer]) n = 1 i = 0
				{
					n_flag = 1;
					inst_form = opcode_find(temp2);
					if(Find_Symbol(temp3 + 1) != -1) // #+symbol
					{
						TA = Find_Symbol(temp3 + 1);
						if(TA - PC >= -2048 && TA - PC <= 2047)  //PC addressing
						{
							p_flag = 1;
							displacement = TA - PC;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;
						}
						else if(base_flag == 1 &&(TA - BASE >= 0 && TA - BASE <= 4095 ))  //BASE addressing
						{
							b_flag = 1;
							displacement = TA - BASE;
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=1 p=0 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%06X",objcode);	
							obj_length += inst_form;				
						}
						else  
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag = 1; 
							continue;
						}
					}
				}
				else // 2 register or buffer,X
				{
					/* 1 register */
					inst_form = opcode_find(temp2);

					if(strcmp(temp3,"A") == 0 || strcmp(temp3,"X") == 0 || strcmp(temp3,"B") == 0 || strcmp(temp3,"S") == 0 || strcmp(temp3,"T") == 0 ||  strcmp(temp3,"F") == 0 || strcmp(temp3,"L") == 0)
					{
						if(opcode_find(temp2) == 2)
						{
							objcode = format2(get_opcode(temp2),temp3[0],0);
							fprintf(fp2,"%3d\t%04X\t%s\t\t%04X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%04X",objcode);	
							obj_length += inst_form;
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag =1;
							continue;
						}
					}
					else  /* 2 register or buffer,X*/
					{
						/* initailize string  */
						strcpy(strreg,temp3);
						strcat(strreg,temp4);
						strcat(strreg,temp5);
						memset(temp3,'\0',strlen(temp3));
						memset(temp4,'\0',strlen(temp4));
						memset(temp5,'\0',strlen(temp5));
						for(i = 0 ; i < strlen(strreg) ; i++)
						{		
							if(strreg[i] == ',')
								break;
							temp3[i] = strreg[i];	
						}
						temp4[0] = strreg[i];
						strcpy(temp5,strreg+i+1);
						inst_form = opcode_find(temp2);
						/* 2 register */
						if(inst_form == 2 && strcmp(temp4,",") == 0)
						{
							if(strcmp(temp3,"A") == 0 || strcmp(temp3,"X") == 0 || strcmp(temp3,"B") == 0 || strcmp(temp3,"S") == 0 || strcmp(temp3,"T") == 0 || strcmp(temp3,"F") == 0 || strcmp(temp3,"L") == 0)
							{
								if(strcmp(temp5,"A") == 0 || strcmp(temp5,"X") == 0 || strcmp(temp5,"B") == 0 || strcmp(temp5,"S") == 0 || strcmp(temp5,"T") == 0 || strcmp(temp5,"F") == 0 || strcmp(temp5,"L") == 0)
								{
									objcode = format2(get_opcode(temp2),temp3[0],temp5[0]);
									fprintf(fp2,"%3d\t%04X\t%s\t%04X\n",line,LOCCTR[ctrcnt],str,objcode);
									if(obj_length + inst_form > 30 || obj_length == 0 )
									{
										fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
										obj_length = 0;			
										idx++;
									}
									fprintf(fp3,"%04X",objcode);	
									obj_length += inst_form;

								}
								else
								{
					printf("Invalid operation code : Line %d\n",line);
									ctrcnt++;
									flag =1;
									continue;
								}
							}
							else
							{
					printf("Invalid operation code : Line %d\n",line);
								ctrcnt++;
								flag =1;
								continue;
							}
						}
						/* BUFFER, X*/
						else if(strcmp(temp4,",") == 0 && strcmp(temp5,"X") == 0)
						{
							x_flag = 1;
							n_flag = 1;
							i_flag = 1;
							TA = Find_Symbol(temp3);	
							if(TA != -1 && inst_form ==3 )
							{
								if( TA - PC >= -2048 && TA - PC <= 2047)
								{
									p_flag = 1;
									displacement = TA - PC;
									nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
									objcode = format34(get_opcode(temp2),nixbpe,displacement);
									fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
									if(obj_length + inst_form > 30 || obj_length == 0 )
									{
										fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
										obj_length = 0;			
										idx++;
									}
									fprintf(fp3,"%06X",objcode);	
									obj_length += inst_form;


								}
								else if (TA - BASE >= 0 && TA - BASE <= 4095)
								{
									b_flag = 1;
									displacement = TA - BASE;
									nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
									objcode = format34(get_opcode(temp2),nixbpe,displacement);
									fprintf(fp2,"%3d\t%04X\t%s\t%06X\n",line,LOCCTR[ctrcnt],str,objcode);
									if(obj_length + inst_form > 30 || obj_length == 0 )
									{
										fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
										obj_length = 0;			
										idx++;
									}
									fprintf(fp3,"%06X",objcode);	
									obj_length += inst_form;


								}
								else
								{
					printf("Invalid operation code : Line %d\n",line);
									ctrcnt++;
									flag =1;
									continue;
								}
							}
							else
							{
					printf("Invalid operation code : Line %d\n",line);
								ctrcnt++;
								flag =1;
								continue;
							}
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag =1;
							continue;
						}
					}
				}
				ctrcnt++;	
			}
		}
		else if(Find_Symbol(temp1) != -1 && opcode_find(temp2) == 4) //label exist, format = 4 , temp1 = LABEL temp2 = OPCODE
		{
			if(Find_Symbol(temp1) != -1)
			{
				inst_form = 4;
				e_flag = 1;
				if(Find_Symbol(temp3) != -1 && temp4[0] == '\0')  //symbol exist
				{
					TA = Find_Symbol(temp3);
					if(TA != -1)
					{	
						modification[cnt] = LOCCTR[ctrcnt] -address + 1;
						cnt++;

						n_flag = 1;
						i_flag = 1;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp2),nixbpe,TA);
						fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%08X",objcode);	
						obj_length += inst_form;
					}
					else
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag =1;
						continue;
					}
				}
				else if(*temp3 == '@') // if first letter of operand is @
				{

					TA = Find_Symbol(temp3+1);

					if(TA != -1)
					{
						modification[cnt] = LOCCTR[ctrcnt] -address + 1;
						cnt++;

						n_flag = 1;
						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp2),nixbpe,TA);
						fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%08X",objcode);	
						obj_length += inst_form;
					}
					else 
					{
					printf("Invalid operation code : Line %d\n",line);
						ctrcnt++;
						flag =1;
						continue;
					}
				}
				else if(*temp3 == '#') // if first letter of operand is #
				{
					i_flag = 1;
					TA = Find_Symbol(temp3+1);  
					if(TA != -1)   //operand is Symbol
					{
						modification[cnt] = LOCCTR[ctrcnt] -address + 1;
						cnt++;

						nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
						objcode = format34(get_opcode(temp2),nixbpe,TA);	
						fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
						if(obj_length + inst_form > 30 || obj_length == 0 )
						{
							fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
							obj_length = 0;			
							idx++;
						}
						fprintf(fp3,"%08X",objcode);	
						obj_length += inst_form;
					}
					else  		//operand is num
					{
						sscanf(temp3+1,"%d",&displacement);
						if(displacement >= 0 && displacement <=65535)
						{
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,displacement);			// n=0 i=1 x=0 b=0 p=1 e=0
							fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%08X",objcode);	
							obj_length += inst_form;
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag = 1;
							continue;
						}

					}
				}
				else  /*Buffer,X */
				{
					modification[cnt] = LOCCTR[ctrcnt] -address + 1;
					cnt++;

					n_flag = 1;
					i_flag = 1;
					x_flag = 1;
					/* initailize string  */
					strcpy(strreg,temp3);
					strcat(strreg,temp4);
					strcat(strreg,temp5);
					memset(temp3,'\0',strlen(temp3));
					memset(temp4,'\0',strlen(temp4));
					memset(temp5,'\0',strlen(temp5));
					for(i = 0 ; i < strlen(strreg) ; i++)
					{		
						if(strreg[i] == ',')
							break;
						temp3[i] = strreg[i];	
					}
					temp4[0] = strreg[i];
					strcpy(temp5,strreg+i+1);
					inst_form = opcode_find(temp2);

					if(strcmp(temp4,",") == 0 && strcmp(temp5,"X") == 0)
					{
						x_flag = 1;
						n_flag = 1;
						i_flag = 1;
						TA = Find_Symbol(temp3);	
						if(TA != -1 && inst_form ==3 )
						{
							nixbpe = e_flag*1 + p_flag*2 + b_flag*4 + x_flag*8 + i_flag*16 + n_flag*32; 
							objcode = format34(get_opcode(temp2),nixbpe,TA);
							fprintf(fp2,"%3d\t%04X\t%s\t%08X\n",line,LOCCTR[ctrcnt],str,objcode);
							if(obj_length + inst_form > 30 || obj_length == 0 )
							{
								fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
								obj_length = 0;			
								idx++;
							}
							fprintf(fp3,"%08X",objcode);	
							obj_length += inst_form;
						}
						else
						{
					printf("Invalid operation code : Line %d\n",line);
							ctrcnt++;
							flag =1;
							continue;
						}

					}
				}
			}
			ctrcnt++;
		}
		else if(strcmp(temp1,"BASE") == 0)  //if opcode is BASE ,set base flag
		{
			base_flag = 1;
			if(Find_Symbol(temp2) != -1)
			{
				BASE = Find_Symbol(temp2);
				fprintf(fp2,"%3d\t\t%s\n",line,str);
				ctrcnt++;
			}
			else
			{
				flag = 1;
					printf("Invalid operation code : Line %d\n",line);
				ctrcnt++;
				continue;
			}
		}
		else if(strcmp(temp2,"WORD") == 0)
		{
			sscanf(temp2,"%d %c",&num,&error);
			if(num >= 0 && (error == '\0'))
			{
				if(num >= 0 && num<=16777215)
				{
					fprintf(fp2,"%d\t%04X\t%s\n",line,LOCCTR[ctrcnt],str);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%06X",num);	
					obj_length += 3;
				}
				else
				{
					flag = 1;
					printf("Invalid operation code : Line %d\n",line);
					ctrcnt++;
					continue;
				}
				ctrcnt++;
			}
			else
			{
				flag = 1;
					printf("Invalid operation code : Line %d\n",line);
				ctrcnt++;
				continue;
			}
		}
		else if(strcmp(temp2,"BYTE") == 0)
		{
			if(*temp3 == 'X')		//  X'string'
			{
				sscanf(temp3,"%c %c %[^'] %c",&error,&error1,str_temp,&error2);
				if(strlen(str_temp) % 2 == 1) //  string length can devide by 2 
				{
					printf("Invalid string size : Line %d\n",line);
					flag = 1;
					ctrcnt++;
					continue;
				}

				if(error == 'X' && error1 == '\'' && error2 == '\'')
				{

					fprintf(fp2,"%3d\t%04X\t%s\t",line,LOCCTR[ctrcnt],str);
					fprintf(fp2,"%s\n",str_temp);
					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt-1],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					fprintf(fp3,"%s",str_temp);
					obj_length += strlen(str_temp)/2;
				}
			}
			else if(*temp3 == 'C')	// C'string'
			{
				sscanf(temp3,"%c %c %[^'] %c",&error,&error1,str_temp,&error2);
				if(strlen(str_temp) > 30)  //string length smaller than 30
				{
					printf("Invalid string size code : Line %d\n",line);
					flag = 1;
					ctrcnt++;
					continue;
				}
				if(error == 'C' && error1 == '\'' && error2 == '\'')
				{
					fprintf(fp2,"%3d\t%04X\t%s\t",line,LOCCTR[ctrcnt],str);
					for(i = 0 ; i< strlen(str_temp) ; i++)
						fprintf(fp2,"%02X",str_temp[i]);
					fprintf(fp2,"\n");

					if(obj_length + inst_form > 30 || obj_length == 0 )
					{
						fprintf(fp3,"\nT%06X%02X",LOCCTR[ctrcnt-1],T_length[idx]);							
						obj_length = 0;			
						idx++;
					}
					for(i = 0; i< strlen(str_temp) ; i++)
						fprintf(fp3,"%X",str_temp[i]);
					obj_length += strlen(str_temp);
				}
			}
			ctrcnt++;
		}
		else if(strcmp(temp2,"RESW") == 0)
		{	
			fprintf(fp2,"%3d\t%04X\t%s\n",line,LOCCTR[ctrcnt],str);
			obj_length = 0;
			ctrcnt++;
		}
		else if(strcmp(temp2,"RESB") == 0)
		{
			fprintf(fp2,"%3d\t%04X\t%s\n",line,LOCCTR[ctrcnt],str);
			obj_length = 0;
			ctrcnt++;
		}
		else 
		{
			flag = 1;
			printf("invalid operation code line : %d\n",line);
			ctrcnt++;
			continue;
		}

	}	

	for(i = 0 ; i < cnt; i++)
		fprintf(fp3,"\nM%06X05",modification[i]);

	fprintf(fp3,"\nE%06X\n",LOCCTR[0]);

	if(flag == 1)
	{
		remove(lstname);
		remove(objname);
	}
	else
	{
		printf("\toutput file : [%s], [%s]\n",lstname,objname);
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

int get_opcode(char *opcode)
{
	hash_table *temp;
	char new_opcode[10];
	if(opcode_find(opcode) == 0)
		return -1;

	if(opcode[0] == '+')
	{
		strcpy(new_opcode,opcode+1);
	}
	else strcpy(new_opcode,opcode);

	for(temp = hashtable[make_hash_func(new_opcode)] ; temp ;temp = temp -> link)
	{
		if(strcmp(temp->mnemonic,new_opcode) == 0)
			return temp->code;
	}

	return -1;
}

int format2(int opcode,char reg1,char reg2)
{
	int sum = 0;
	if( reg1 == 'A')
		reg1 = 0;
	else if( reg1 == 'X' )
		reg1 = 1;
	else if( reg1 == 'L' )
		reg1 = 2;
	else if( reg1 == 'B' )
		reg1 = 3;
	else if( reg1 == 'S' )
		reg1 = 4;
	else if( reg1 == 'T' )
		reg1 = 5;
	else if( reg1 == 'F' )
		reg1 = 6;

	if( reg2 == 'A' )
		reg2 = 0 ;
	else if( reg2 == 'X' )
		reg2 = 1;
	else if( reg2 == 'L' )
		reg2 = 2;
	else if( reg2 == 'B' )
		reg2 = 3;
	else if( reg2 == 'S' )
		reg2 = 4;
	else if( reg2 == 'T' )
		reg2 = 5;
	else if( reg2 == 'F' )
		reg2 = 6;

	opcode <<= 8;
	sum |= opcode;
	reg1 <<= 4;
	sum |= reg1;
	sum |= reg2;

	return sum;
}

int format34(int opcode, int nixbpe, int disp_addr)
{
	int sum=0;
	if( nixbpe%2 == 0)
	{
		disp_addr &= 0xFFF;
		opcode <<= 16;
		sum |= opcode;
		nixbpe <<= 12;
		sum |= nixbpe;
		sum |= disp_addr;
	}
	else 
	{
		disp_addr &= 0xFFFFF;
		opcode <<= 24;
		sum |= opcode;
		nixbpe <<= 20;
		sum |= nixbpe;
		sum |= disp_addr;
	}
	return sum;
}

void mode_type(char *filename)
{
	FILE *fp;
	char str[500];
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("filename is error!!\n");
		return ;
	}

	if(fp != 0)
	{		
		while(fgets(str,500,fp) != NULL)
			printf("%s",str);
	}

	fclose(fp);
}

void mode_progaddr(int prog_address)
{
	run_address = prog_address;
	return ;
}

void mode_loader(char* proga,char* progb,char* progc)
{
	FILE *fp;
	FILE *fp1,*fp2,*fp3;
	int file_index = 0;
	int PROGADDR,CSADDR;  //program address , control section address
	int CSLTH,temp;   //control section length
	char str[100];	//Input string
	char prog_name[10];
	int t,i,r,result;
	int D = 0;
	char symname[10];
	char rec_type;
	char m_operator;
	int obj_code,obj_addr,obj_length;
	int ref_num,ref_index=0;
	int map_index = 0;
	int total_length = 0;
	int mod;
	int temp_addr = 0;
	fp1 = fopen(proga,"r");
	fp2 = fopen(progb,"r");
	fp3 = fopen(progc,"r");
	if(fp1 == NULL ||fp2 == NULL || fp3 == NULL)
	{
		printf("Error file!!\n");
		return ;
	}
	/* set PROGGADDR and CSADDR */
	PROGADDR = run_address;
	CSADDR = PROGADDR;

	/* Pass 1 */ 
	fp = fp1; 
	while(fgets(str,100,fp) && file_index < 3)  
	{	
		sscanf(str,"%c %6s %*6X %6X",&rec_type,prog_name,&CSLTH); //set program name and control section length
		if( search_ESTAB(prog_name) == 1 )     //search ESTAB for Control section name		{
		{	
			printf("Duplicate external symbol\n");
			return ;
		}
		else
		{
			construct_ESTAB(CSADDR,prog_name,CSLTH,rec_type);  //enter control section name into ESTAB
			load_map[map_index].rec_type = 'H';
			strcpy(load_map[map_index].name,prog_name);
			load_map[map_index].addr = CSADDR;
			load_map[map_index].length = CSLTH;
			map_index++;
		}

		while(str[0] != 'E')
		{
			fgets(str,100,fp);
			if(str[0] == 'D')
			{
				D = 0;
				while(1)
				{
					result = sscanf(str+1+D,"%s %6X",symname,&sym_address);		
					if(result != 2)
						break;
					if(search_ESTAB(symname) == 1)   //search ESTAB for symbol name
					{
						printf("Duplicate external symbol\n");
						return ;
					}	
					construct_ESTAB(CSADDR + sym_address,symname,0,'D');  //enter symbol into ESTAB with address
					load_map[map_index].rec_type = 'D';
					strcpy(load_map[map_index].name,symname);
					load_map[map_index].addr = CSADDR + sym_address;
					map_index++;
					D = D + 12;
				}
			}
		}
		CSADDR += CSLTH;  //add CSLTH to CSADDR (starting address for next control section
		
		if(str[0] == 'E') //if end of file, get next control section
		{
			file_index++;
			if(file_index == 1 && fp2 != NULL) 
				fp = fp2;
			else if(file_index == 2 && fp3 != NULL)
				fp = fp3;
			else break;
		}
		memset(str,'\0',100);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	file_index = 0;
	/* Pass 2 */
	PROGADDR = run_address;
	CSADDR = PROGADDR;
	fp1 = fopen(proga,"r");
	fp2 = fopen(progb,"r");
	fp3 = fopen(progc,"r");
	fp = fp1;
	memset(str,'\0',100);
	CSADDR = PROGADDR;  //set CSADDR to PROGADDR
	while(fgets(str,100,fp) && file_index < 3)  // while not end of input
	{
		sscanf(str+1,"%6s %6X %6X",ref[ref_index].name,&temp,&CSLTH); //set CSLTH to control section length
		ref[ref_index++].ref_num = 1;
		while(str[0] != 'E')
		{
			fgets(str,100,fp);
			if(str[0] == 'R')
			{
				r = 0;
				while(1)  //save refernce number
				{
					result = sscanf(str+1+r,"%2X %6s",&ref[ref_index].ref_num,ref[ref_index].name);
					if(result != 2 )
						break;	
					ref_index++;
					r = r + 8;
				}
				
			}
			else if(str[0] == 'T')  //text record
			{
				sscanf(str+1,"%6X %2X",&obj_addr,&obj_length);
				for(i = 0 ; i < obj_length ; i++)    //save object code to memory      
				{
					sscanf(str+9+2*i,"%2X",&obj_code);
					virt_memory[CSADDR + obj_addr + i] = obj_code;
				}
			}
			else if(str[0] == 'M')  //modification record
			{
				sscanf(str+1,"%6X %*2X %c %2X",&obj_addr,&m_operator,&ref_num);
				for( i = 0 ; i < ref_index ; i++)
				{
					if(ref[i].ref_num == ref_num)     //check reference number&calculate modification
					{
						if(search_ESTAB(ref[i].name) == 1)
						{
							if(temp_addr != obj_addr )
							{
								mod = 0;
								for(t = 0 ; t < 3 ; t++)
									mod = mod*16*16 + virt_memory[CSADDR + obj_addr + t];
							}

							if(m_operator == '-')  //if operator is minus
							{
								mod += ~sym_addr + 1;
							}
							else if(m_operator == '+')  //if operator is plus
							{
								mod += sym_addr;
							}
							else
							{
								printf("Error operator!!\n");
								return ;
							}
						}
						else 
						{
							printf("Undefined external symbol\n");
							return ;

						}
						break;
					}
				}
				temp_addr = obj_addr;
				/*save modification to memory*/
				if(mod < 0)
					mod = 16*16*16*16*16*16 - mod;      
			
				virt_memory[CSADDR + obj_addr] = mod/256/256;
				virt_memory[CSADDR + obj_addr + 1] = (mod/256)%256;
				virt_memory[CSADDR + obj_addr + 2] = mod%256;
			}
		}
		CSADDR += CSLTH;
		/* if end of file , get next control section */
		if(str[0] == 'E')
		{
			file_index++;
			if(file_index == 1 && fp2 != NULL)
				fp = fp2;
			else if(file_index == 2 && fp3 != NULL)
				fp = fp3;
			else break;
		}
		memset(str,'\0',100);
		ref_index = 0;
		temp_addr = 0;
	
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	/* if Linking loader is success , print the load map */
	total_length = 0;
	printf("\tcontrol\t\tsymbol\t\taddress\t\tlength\n");
	printf("\tsection\t\tname  \t\t\n");
	printf("\t------------------------------------------------------\n");
	for(i = 0 ; i < map_index ; i++)
	{
		if(load_map[i].rec_type == 'H')
			printf("\t%-8s\t\t\t%04X\t\t%04X\n",load_map[i].name,load_map[i].addr,load_map[i].length);
		else if (load_map[i].rec_type == 'D') 
			printf("\t\t\t%-8s\t%04X\n",load_map[i].name,load_map[i].addr);
	}		
	printf("\t------------------------------------------------------\n");
	for(i = 0 ; i < map_index ; i++)
	{
		if(load_map[i].rec_type == 'H')
			total_length += load_map[i].length;
	}
	temp_addr = 0;
	sym_addr = 0;
	ref_index = 0;
	for( i = 0 ;i < 1000; i++)
	{
		ref[i].ref_num = 0 ;
		memset(ref[i].name,'\0',10);
	}
	printf("\t\t\t\t\tLENGTH :  \t%04X\n",total_length);
	return ;
}

void construct_ESTAB(int CSADDR,char *name,int length,char rec_type)
{	
	int hash_num;
	ES_TAB *new1,*new2;

	new1 = (ES_TAB*)malloc(sizeof(ES_TAB));
	hash_num = make_hash_func(name);
	new1->CSADDR = CSADDR;
	strcpy(new1->name,name);
	new1->length = length;
	new1->rec_type = rec_type;
	new1->link = NULL;   
	if(!ESTAB[hash_num])    //if ESTAB[hash_num] is empty
		ESTAB[hash_num] = new1;
	else					
	{
		for(new2 = ESTAB[hash_num] ; new2 -> link ; new2 = new2 ->link)
			;
		new2->link = new1;
	}
}

int search_ESTAB(char *name)
{	
	int hash_num;
	ES_TAB *new1;
	hash_num = make_hash_func(name);
	for(new1 = ESTAB[hash_num] ; new1 != NULL ; new1 = new1->link)
	{
		if(strcmp(new1->name,name) == 0)
		{
		    sym_addr = new1->CSADDR;
			return 1;
		}
	}
	return -1;
}


