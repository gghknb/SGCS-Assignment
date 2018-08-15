#include "20141575.h"

int main()
{
	
	char mode[80];
	int index = 0;
	char dump[10],edit[10],fill[10],opcode[10];
	char comma,comma2,comma3;
	int start ,end;
	int result,result2;
	int address;
	int value;
	char opcode_inst[100];
	FILE *fp;
	char str[80];

	/* open opcode txt and make hash code table*/
	fp = fopen("opcode.txt","r");
	construct_hashtable(fp);
	fclose(fp);

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
		for(print_node = hashtable[i] ; print_node ; print_node = print_node->link)
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
	for(temp = hashtable[hash_num] ; temp->link ; temp = temp->link)
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
		hashtable[i] = (hash_table*)malloc(sizeof(hash_table*));
		hash_head = hashtable[i];
		for(j = 0 ; j < 58 ; j++)
		{
			if(i == opcode[j].hash_num)
			{
				new1 = (hash_table*)malloc(sizeof(hash_table));
				strcpy(new1->mnemonic,opcode[j].mnemonic);
				new1->code = opcode[j].code;
				new1->link = NULL;
				if(!hash_head)
					hash_head = new1;
				else
				{
					for(new2 = hash_head ; new2->link ; new2 = new2->link)
						;
					new2->link = new1;
				}
			}
		}
		hashtable[i] = hash_head -> link;
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





















