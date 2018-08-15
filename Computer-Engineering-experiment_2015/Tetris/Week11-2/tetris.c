
#include "tetris.h"

static struct sigaction act, oact;

typedef struct DataNode* DataPointer;
typedef struct DataNode{
	char name[100];
	int score1;
	DataPointer leftlink;
	DataPointer rightlink;
	}DataNode;

DataPointer head_data;



int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
	createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_AUTO: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawShadow(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	//Draw the space outline
	DrawBox(0,0,HEIGHT,WIDTH);

	//Draw the space where the next block space
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	move(8,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);
	//Draw the space where the score showing
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	case -1:
		drawFlag=0;
		DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
		if(CheckToMove(field,nextBlock[0],recommendR,recommendY,recommendX)==0){
			drawFlag=1;
			blockY++;
			command=KEY_DOWN;
		
			break;
		}
		AddBlockToField(field,nextBlock[0],recommendR,recommendY,recommendX);
		score+=DeleteLine(field,0);
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		nextBlock[2]=rand()%7;
		blockRotate=0;
		blockY=-1;
		blockX=WIDTH/2-2;
		timed_out=0;
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		getchar();
		break; 
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for(i=0;i<4;i++){
		move(10+i,WIDTH+13);
		for(j=0;j<4;j++){
			if(block[nextBlock[2]][0][i][j]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				
				if(i+blockY>HEIGHT-1)
					return 0;	
				if(j+blockX<0||j+blockX>WIDTH-1)
					return 0;	
				if(field[i+blockY][j+blockX]==1)
					return 0;
			}
		}
	}
	return 1;

}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int temp_blockX, temp_blockY, temp_blockRotate;
	int i, j;
	int flag=1;

	temp_blockX=blockX;
	temp_blockY=blockY;
	temp_blockRotate=blockRotate;

	switch(command){
	case KEY_UP:
		if(temp_blockRotate==0)
			temp_blockRotate=3;
		else
			temp_blockRotate=temp_blockRotate-1;
		break;
	case KEY_DOWN:
		temp_blockY--;
		break;
	case KEY_LEFT:
		temp_blockX++;
		break;
	case KEY_RIGHT:
		temp_blockX--;
		break;
	}

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][temp_blockRotate][i][j]==1){
				move(i+temp_blockY+1,j+temp_blockX+1);
				printw(".");
			}
		}
	}

	while(flag==1){
		temp_blockY++;
		flag=CheckToMove(field,currentBlock,temp_blockRotate,temp_blockY,temp_blockX);
		if(flag==0){
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					if(block[currentBlock][temp_blockRotate][i][j]==1){
						move(i+temp_blockY,j+temp_blockX+1);
						printw(".");
					}
				}
			}
		}
	}

	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
	move(HEIGHT,WIDTH+10);
	
}

void BlockDown(int sig){
	// user code
	int next_flag;
	int command;

	command=KEY_DOWN;
	next_flag=CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX);
	if(next_flag==1){
		blockY++;
		DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
		timed_out=0;
	}
	if(next_flag==0&&blockY==-1){
		gameOver=1;
	}
	else if(next_flag==0&&blockY!=-1){
		AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);
		score+=DeleteLine(field,0);
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		nextBlock[2]=rand()%7;
		blockRotate=0;
		blockY=-1;
		blockX=WIDTH/2-2;
		timed_out=0;
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		
	}
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	int touched=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				field[blockY+i][blockX+j]=1;
				if(field[blockY+i+1][blockX+j]==1)
					touched++;				
				if(blockY+i==HEIGHT-1)
					touched++;
			}
		}
	}
	score=score+touched*10;
}

int DeleteLine(char f[HEIGHT][WIDTH], int mode){
	// user code
	int line_return=0;
	int i, j, k,l;
	int flag[HEIGHT]={0};
	int temp_field[HEIGHT][WIDTH]={0};


	for(i=HEIGHT-1;i>0;i--){
		for(j=0;j<WIDTH;j++){
			if(f[i][j]!=1)
				j=WIDTH;
			if(j==WIDTH-1){
				line_return++;
				flag[i]=1;
			}
		}
	}
	if(line_return>0){
		for(l=0;l<line_return;l++){

			for(i=HEIGHT-1;i>0;i--){
				if(flag[i]==1){
					for(j=i;j<HEIGHT;j++){
						for(k=0;k<WIDTH;k++){
							if(mode==0)
								temp_field[j][k]=field[j][k];
							else
								temp_field[j][k]=f[j][k];
						}
					}
					for(j=i;j>0;j--){
						for(k=0;k<WIDTH;k++){
							if(mode==0)
								temp_field[j][k]=field[j-1][k];
							else
								temp_field[j][k]=f[j-1][k];
						}

					}
					for(j=0;j<HEIGHT;j++){
						for(k=0;k<WIDTH;k++){
							if(mode==0)
								field[j][k]=temp_field[j][k];
							else
								f[j][k]=temp_field[j][k];
						}
					}
				}	
			}
	
		}
	}

	line_return=line_return*line_return*100;
	return line_return;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user codei
	int temp_blockY;
	int flag=1;
	int i,j;
	
	temp_blockY=y;
	
	while(flag==1){
		
		temp_blockY++;
		flag=CheckToMove(field,blockID,blockRotate,temp_blockY,x);
		if(flag==0){
			DrawBlock(temp_blockY-1,x,blockID,blockRotate,'/');
		}
	}
	move(HEIGHT,WIDTH+10);
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	_Node rec;	
	int i, j;
	DrawBlock(y,x,blockID,blockRotate,' ');
	DrawShadow(y,x,blockID,blockRotate);
	rec=(_Node)malloc(sizeof(Node));
	rec->level=0;
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			rec->recField[i][j]=field[i][j];
		}
	}
	modified_recommend(rec);
}
void createRankList(){
	// user code

	int num=0;
	DataPointer move_pointer,temp;

	FILE *fp;

	fp=fopen("rank.txt", "r");
	fscanf(fp,"%d",&num);
	rank_num=num;
	if(num<0){
		printw("FILE OPEN ERROR!\n");
		exit(-1);
	}
	if(num>0){
		move_pointer=(DataPointer)malloc(sizeof(DataNode));
		head_data=move_pointer;
		move_pointer->leftlink=NULL;
		temp=move_pointer;
		fscanf(fp,"%s %d", move_pointer->name, &move_pointer->score1);
		num--;
		while(num!=0){
			move_pointer=(DataPointer)malloc(sizeof(DataNode));
			fscanf(fp,"%s %d", move_pointer->name, &move_pointer->score1);
			num--;
			temp->rightlink=move_pointer;
			move_pointer->leftlink=temp;
			move_pointer->rightlink=NULL;
			temp=move_pointer; 
		}
		head_data->leftlink=temp;
	}
	fclose(fp);
	
}

void rank(){
	// user code

	clear();
	system("clear");
	int num1=0, num2=0;
	int compare_num;
	int use;
	int counter, start=0;
	char input_name[100];
	int delete_rank=0;

	DataPointer output;
	echo();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks from by a specific name\n");
	printw("3. delete a specific rank\n");

	scanw("%d", &use);

	if(use==1){
		echo();
		printw("X:");
		scanw("%d", &num1);
		echo();
		printw("Y:");
		scanw("%d", &num2);
		printw("\n\n");
		output=head_data;
		printw("==============================\n");
		printw("|      name      |   score   |\n");
		printw("==============================\n");
		if(num1==0)
			num1=1;
		if(num2==0)
			num2=5;
		if(num1>num2||num1<1||num2>rank_num)
			printw("SEARCH FAILURE: NO RANK IN THE LIST\n");
		else{
			compare_num=rank_num/2;
			if(compare_num>=num1){
				for(counter=1;counter<=num2;counter++){
					if(counter==num1)
					start=1;
					if(start==1){
						printw("|%12s     |     %5d|\n",output->name, output->score1);
					} 
					output=output->rightlink;
				}
			}
			else{
				output=output->leftlink;
				for(counter=rank_num;counter>=num1;counter--)
					output=output->leftlink;
				for(;start<=num2-num1;start++){
					output=output->rightlink;
					printw("|%12s     |    %5d|\n", output->name, output->score1);
				}
			}
			printw("===============================\n");
		}
		
	}
	else if(use==2){
		echo();
		printw("Input the name: ");
		scanw("%s", input_name);
		echo();
		printw("===============================\n");
		printw("|	name	|    score    |\n");
		printw("===============================\n");

		for(output=head_data;output!=NULL;output=output->rightlink){
			echo();
			if(strcmp(output->name,input_name)==0){
				num1++;
				printw("|%12s   |        %5d|\n", output->name, output->score1);
			}
		}
		if(output==NULL){
			echo();
			if(num1==0)
				printw("SEARCH FAILURE: NO NAME IN THE LIST\n");
			else
				printw("===============================\n");
		}
		
	}
	else if(use==3){
		echo();
		printw("Input the rank : ");
		scanw("%d", &delete_rank);

		if(delete_rank>rank_num||delete_rank<0)
			printw("SEARCH FAILURE: THE RANK NOT IN THE LIST\n");
		else{
			compare_num=rank_num/2;
			output=head_data;
			if(compare_num>=num1){
				for(num1=1;delete_rank>num1;num1++)
					output=output->rightlink;
				}
			else{
				output=output->leftlink;
				for(num1=rank_num;delete_rank>num1;num1--)
					output=output->leftlink;
			}

			if(delete_rank==rank_num){
				output=head_data->leftlink->leftlink;
				head_data->leftlink=output;
				output->rightlink=NULL;
			}
			else{
				output->leftlink->rightlink=output->rightlink;
				output->rightlink->leftlink=output->leftlink;
				free(output);
			}
			rank_num--;
			printw("result : the rank deleted\n");
			writeRankFile();	
		}
	}
	else
		printw("INPUT ERROR\n");

	getch();

}

void writeRankFile(){
	// user code
	DataPointer temp;
	FILE *fp;
	fp=fopen("rank.txt","w");
	temp=head_data;
	fprintf(fp,"%d\n",rank_num);
	while(temp!=NULL){
		fprintf(fp, "%s	%d\n", temp->name, temp->score1);
		temp=temp->rightlink;
	}
	fclose(fp);
}

void newRank(int score){
	// user code

	int temp_num;
	int i;
	DataPointer user, temp, pretemp;
	system("clear");

	user=(DataPointer)malloc(sizeof(DataNode));
	echo();
	printw("YOUR NAME");
	scanw("%s", user->name);

	user->score1=score;
	user->rightlink=NULL;
	
	if(head_data->score1<score){
		head_data->leftlink=user;
		user->rightlink=head_data;
		head_data=user;
		rank_num++;
		head_data=temp;
		while(temp->rightlink!=NULL)
			temp=temp->rightlink;
		head_data->leftlink=temp;	
	}
	else{
		temp_num=rank_num;
		pretemp=head_data;
		temp=head_data;
	
		while(temp_num!=0){
			temp_num--;
			if(temp->score1<score){
				temp->leftlink=user;
				user->rightlink=temp;
				user->leftlink=pretemp;
				pretemp->rightlink=user;
				break;
			}
			pretemp=temp;
			temp=temp->rightlink;
		}
		if(temp_num==0){
			user->leftlink=pretemp;
			user->rightlink=NULL;
			pretemp->rightlink=user;
			head_data->leftlink=user;
		}
		rank_num++;
	}
	
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	DrawBlock(y,x,blockID,blockRotate,'R');
}


int recommend(_Node root){
	char fieldNew[HEIGHT][WIDTH];
	int field_info[WIDTH];
	int i, j, k, l;
	int r,x,y=1;
	Node **c;
	int rotating_num;
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int touched=0, temp_score=0;
	int MAX_LEVEL=2;
	int result_r=0, result_x=0, result_y=0;
	int flag=1;
	c=root->child;
	c=(Node**)malloc(sizeof(struct Node)*40);

	for(i=0;i<=39;i++){
		c[i]=(_Node)malloc(sizeof(Node));
		c[i]->level=root->level+1;
		c[i]->curBlockID=nextBlock[root->level];
	}

	i=0;

	for(rotating_num=0;rotating_num<4;rotating_num++){
		for(x=-1;x<=WIDTH;x++){
			flag=1;			
			y=1;

			if(CheckToMove(root->recField,c[i]->curBlockID,rotating_num,y,x)==0){
				flag=0;
			}
			while(flag==1){
				y++;
				flag=CheckToMove(root->recField,c[i]->curBlockID,rotating_num,y,x);
				if(flag==0){
			
					y--;
					i++;
			
			
					//1. child[]->field=root->field
					for(j=0;j<HEIGHT;j++){
						for(k=0;k<WIDTH;k++)	
							c[i]->recField[j][k]=root->recField[j][k];
					}
					temp_score=0;
					touched=0;
					//2. If added on child field, calculate score
					for(j=0;j<4;j++){
						for(k=0;k<4;k++){
							if(block[c[i]->curBlockID][rotating_num][j][k]==1){
								c[i]->recField[y+j][x+k]=1;
								if(c[i]->recField[y+j+1][x+k]==1)
									touched++;				
								if(y+j==HEIGHT-1)
									touched++;
							}
						}
					}
					temp_score=temp_score+touched*10;
					temp_score+=DeleteLine(c[i]->recField,1);
					//3.Two situation . If leaf is node or not.
					if(c[i]->level!=MAX_LEVEL){
						temp_score+=recommend(c[i]);
					}


					//4.if temp_score is bigger than MAX value
					if(max<temp_score){
						max=temp_score;
						result_x=x;
						result_y=y;
						result_r=rotating_num;
					}
					if(max==temp_score&&result_y<y){
						max=temp_score;
						result_x=x;
						result_y=y;
						result_r=rotating_num;
					}

				}
			}
		}
	}

	
	// user code
	if(root->level==0){
		DrawRecommend(result_y,result_x, nextBlock[0],result_r);
	}


	return max;
}
int modified_recommend(_Node root){
	char fieldNew[HEIGHT][WIDTH];
	int field_info[WIDTH];
	int i, j, k, l;
	int r,x,y=1;
	Node **c;
	int rotating_num;
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int touched=0, temp_score=0;
	int MAX_LEVEL=3;
	int result_r=0, result_x=0, result_y=0;
	int flag=1;

	Node *simple_root[6];
	for(j=0;j<6;j++){
		simple_root[j]=(_Node)malloc(sizeof(Node));
		simple_root[j]->accumulatedScore=-1;
		simple_root[j]->parent=NULL;
	}
	c=root->child;
	c=(Node**)malloc(sizeof(struct Node)*40);

	for(i=0;i<=39;i++){
		c[i]=(_Node)malloc(sizeof(Node));
		c[i]->level=root->level+1;
		c[i]->curBlockID=nextBlock[root->level];
	}

	i=0;

	for(rotating_num=0;rotating_num<4;rotating_num++){
		for(x=-1;x<=WIDTH;x++){
			flag=1;			
			y=1;

			if(CheckToMove(root->recField,c[i]->curBlockID,rotating_num,y,x)==0){
				flag=0;
			}
			while(flag==1){
				y++;
				flag=CheckToMove(root->recField,c[i]->curBlockID,rotating_num,y,x);
				if(flag==0){
			
					y--;
					i++;
			
			
					//1. child[]->field=root->field
					for(j=0;j<HEIGHT;j++){
						for(k=0;k<WIDTH;k++)	
							c[i]->recField[j][k]=root->recField[j][k];
					}
					temp_score=0;
					touched=0;
					//2. If added on child field, calculate score
					for(j=0;j<4;j++){
						for(k=0;k<4;k++){
							if(block[c[i]->curBlockID][rotating_num][j][k]==1){
								c[i]->recField[y+j][x+k]=1;
								if(c[i]->recField[y+j+1][x+k]==1)
									touched++;				
								if(y+j==HEIGHT-1)
									touched++;
							}
						}
					}
					temp_score=temp_score+touched*10;
					temp_score+=DeleteLine(c[i]->recField,1);

					//3. Store the Maximum 3 values on simple_root[j]
					if(c[i]->level!=MAX_LEVEL){
						for(j=0;j<6;j++){
							if(temp_score>simple_root[j]->accumulatedScore){
								simple_root[j]->accumulatedScore=temp_score;
								simple_root[j]->parent=c[i];
								simple_root[j]->recBlockX=x;
								simple_root[j]->recBlockY=y;
								simple_root[j]->recBlockRotate=rotating_num;
								break;
							}
						}
					}
					else{
						if(max<temp_score){
							max=temp_score;
							result_x=x;
							result_y=y;
							result_r=rotating_num;
						}

					}

				}
			}
		}
	}

	if(c[i]->level!=MAX_LEVEL){
		for(j=0;j<6;j++){
			simple_root[j]->accumulatedScore+=modified_recommend(simple_root[j]->parent);
			if(max<simple_root[j]->accumulatedScore){
				max=simple_root[j]->accumulatedScore;
				result_x=simple_root[j]->recBlockX;
				result_y=simple_root[j]->recBlockY;
				result_r=simple_root[j]->recBlockRotate;	
			}			
			if(max==simple_root[j]->accumulatedScore&&result_y<simple_root[j]->recBlockY){
				max=simple_root[j]->accumulatedScore;
				result_x=simple_root[j]->recBlockX;
				result_y=simple_root[j]->recBlockY;
				result_r=simple_root[j]->recBlockRotate;	
			}			
		}
	}

	
	// user code
	if(root->level==0){
		recommendR=result_r;
		recommendY=result_y;
		recommendX=result_x;
		DrawRecommend(result_y,result_x, nextBlock[0],result_r);
	}


	return max;
}

void recommendedPlay(){
	// user code

	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{

		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		ProcessCommand(-1);
	}while(!gameOver);

	alarm(0);
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	getch();
	refresh();
	getch();
}
