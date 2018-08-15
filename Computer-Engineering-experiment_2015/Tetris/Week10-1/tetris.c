#include "tetris.h"
static struct sigaction act, oact;

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
		case MENU_RANK: rank();	break;
		case MENU_EXIT: exit=1;
						writeRankFile();
						break;
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
	nextBlock[2]=rand()%7; //두번째 다음 블록의 ID를 임의로 생성한다.
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* second next block을 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SECOND NEXT BLOCK");
	DrawBox(10,WIDTH+10,4,8);
	/* score를 보여주는 공간의 태두리를 그린다 */
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

	/* 위에 그린 블록의 초기위치를 변경하여 두번째 다음 블록을 그린다. */
	move(11+i,WIDTH+13);
	for(j=0;j<4;j++)
	{
		if(block[nextBlock[2]][0][i][j] ==1) {
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

void DrawBlockWithFeatures(int y,int x,int blockID,int blockRotate)
{
	/* DrawBlock,DrawShadow의 함수를 호출 하는 함수 */
	DrawShadow(y,x,blockID,blockRotate);
	DrawBlock(y,x,blockID,blockRotate,' ');
}

void DrawShadow(int y,int x,int blockID,int blockRotate)
{
	/* 현재 블록을 가장 아래로 내렸을 때 더 이상 내려갈 수 없는 위치를 그림자의 위치로 결정 */
	while(1)
	{
		if(y>=HEIGHT)
			break;
		if(CheckToMove(field,nextBlock[0],blockRotate,y,x) == 0)
			break;
		y++;
	}
	/* 그림자를 그려줌 */
	DrawBlock(y,x,blockID,blockRotate,'/');
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
 //1주차 실습
int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j;
	
	for(i=0;i<BLOCK_HEIGHT;i++)
	{
		for(j=0;j<BLOCK_WIDTH;j++)
		{
			if(block[currentBlock][blockRotate][i][j]==1&&f[blockY+i+1][blockX+j]==1)
				return 0;
			if(block[currentBlock][blockRotate][i][j]==1&&blockY+i>=HEIGHT-1) 
				return 0;
			if(block[currentBlock][blockRotate][i][j]==1&&blockX+j>=WIDTH)
				return 0;
			if(block[currentBlock][blockRotate][i][j]==1&&blockX+j<0)
				return 0;
		}
	}
	return 1;
	// user code
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// command를 받기전에 초기화 (블록을 지움)
	/*switch(command)
	{
		case KEY_UP :initblock(currentBlock,blockRotate-1,blockY,blockX);					  
			break;
		case KEY_DOWN : initblock(currentBlock,blockRotate,blockY-1,blockX);
			break;
		case KEY_LEFT : initblock(currentBlock,blockRotate,blockY,blockX+1);		
			break;
		case KEY_RIGHT : initblock(currentBlock,blockRotate,blockY,blockX-1);
			break;
		default : initblock(currentBlock,blockRotate,blockY-1,blockX);
			break;
	}
	
	*/
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
	// user code
}

void initblock(int currentBlock, int blockRotate,int y,int x){
	int i,j;
	if(blockRotate<0)
		blockRotate=3;
	for(i=0;i<BLOCK_HEIGHT;i++)
	{
		for(j=0;j<BLOCK_WIDTH;j++)
		if(block[currentBlock][blockRotate][i][j]==1&&y+i>=0)
		{
			move(y+i+1,x+j+1);
			printw(".");
		}
	}
	
}


void BlockDown(int sig){
 
	if(CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX)==1)
	{
			blockY++;
			DrawChange(field,-1,nextBlock[0],blockRotate,blockY,blockX);
	}
	else
	{
		if(blockY== -1)
		{
			gameOver = 1;
		}

		score+=AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);

		
		score+=DeleteLine(field);
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		nextBlock[2]= rand()%7; // 블록을 더 내릴수 없을때첫번째 블록은 다음블록 현재블록은 다음블록 2번째블록은 랜덤으로 새로운 id를 갖게함 
		DrawNextBlock(nextBlock);
		PrintScore(score);
		blockRotate=0;
		blockY=-1;
		blockX=WIDTH/2-2;
		DrawField();
	}
	timed_out=0;	

	// user code*/
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j;
	int touched=0; 
	for(i=0;i<BLOCK_HEIGHT;i++)
	{
		for(j=0;j<BLOCK_WIDTH;j++)
		{
			if(block[currentBlock][blockRotate][i][j] == 1)
			{
				f[blockY+i][blockX+j] = 1;
				if(f[i+blockY+1][j+blockX] ==1 || i+blockY+1>=HEIGHT) //맞닿은 부분
					touched++;
			}
		}
	}
	return touched * 10 ;
	// user code
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	int i,j,k;
	int scoretemp,cnt=0;
	int flag;
	for(i=HEIGHT-1;i>0;i--)
	{
		flag=0;
		for(j=0;j<WIDTH;j++)
		{
			if(f[i][j] == 0)
			{
				flag=1;	
				break;
			}
		}
		if(flag==0)
		{
			cnt++;
			for(j=i;j>0;j--)
			{
				for(k=0;k<WIDTH;k++)
				{
					f[j][k]=f[j-1][k];
				}
			}	
			i++;
		}

	}
	scoretemp=cnt*cnt*100;
	return scoretemp;
	// user code*/

}
 

void createRankList(){
	FILE *fp;
	char tempname[NAMELEN];
	char str[50];
	int n,i,tempscore;

	fp=fopen("rank.txt","r");
	fscanf(fp,"%d",&user_num);
	fgetc(fp);
	for(i=0;i<user_num;i++)
	{
		fgets(str,50,fp);
		sscanf(str,"%s %d",tempname,&tempscore);
		inserttolink(tempname,tempscore);
	}

	fclose(fp);

	// user code
}
void inserttolink(char name[NAMELEN],int score)
{
	Node* tmp;
	Node* new;
	new=(Node *)malloc(sizeof(Node));
	new->link=NULL;
	new->score=score;
	strcpy(new->name,name);
	tmp=head;
	if(!head)
	{
		head=new;
	}
	else
	{
		while(tmp->link!=NULL)
		{
			tmp=tmp->link;
		}
		tmp->link=new;
	}
}
void rank(){
	int x=0,y=0,i,t=0;
	int realx,realy;
	int count;
	Node *tmp;
	clear();
	move(0,0);
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	switch(wgetch(stdscr)) {
	case '1':
		echo();	
		printw("X: ");
		scanw("%d",&x);
		printw("Y: ");
		scanw("%d",&y);
		
		
		if(x<40 && x>=0)
			realx=x;
		if(y<40 && y>=0)
			realy=y;
		
		printw("       name      |     score     \n");
		printw("---------------------------------\n");
		
		//createRankList();
		if(realx>realy && realy!=0)
		{
			printw("search failure: no rank in the list\n");
			break;
		}
		else if(realx<0||realy>user_num) printw("search failure: no rank in the list\n");

		else 
		{
			if(realx==0 && realy!=0)
			{
				tmp=head;
				while(tmp!=NULL && realy>0 )
				{
					printw("%10s\t\t%8d\n",tmp->name,tmp->score);
					tmp = tmp->link;
					realy--;
				}
				break;
			}
			else if(realx!=0  && realy==0)
			{
				tmp=head;
				while(realx>1)
				{
					tmp=tmp->link;
					realx--;
				}
				while(tmp != NULL)
				{
					printw("%10s\t\t%8d\n",tmp->name,tmp->score);
					tmp = tmp->link;
					t++;
				}
			
				break;
			}
			else if(realx==0 &&realy==0)
			{	
				tmp=head;
				while(tmp!=NULL)
				{
					printw("%10s\t\t%8d\n",tmp->name,tmp->score);
					tmp = tmp->link;
				}
				break;
			}
			else
			{
				tmp=head;
				count=realy-realx+1;	
				while(realx>1)
				{
					tmp=tmp->link;
					realx--;
				}
				for(i=0;i<count;i++)
				{
					printw("%10s\t\t%8d\n",tmp->name,tmp->score);
					tmp= tmp->link;
				}
				break;
			}

			
		}
	case '2' : break;
	case '3' : break;

	};
	noecho();
	getch();
	// user code
}

void writeRankFile(){
	FILE *fp;
	int n;
	Node *tmp;
	tmp=head;
	fp=fopen("rank.txt","w");

	fprintf(fp,"%d\n",user_num);
	while(tmp)
	{
		fprintf(fp,"%s %d\n",tmp->name,tmp->score);
		tmp=tmp->link;
	}
	fclose(fp);

	// user code
}

void newRank(int score){
	char tempname[20];
	Node *new;
	Node *tmp;
	clear();
	echo();
	printw("Input a user name : ");
	scanw("%s",tempname);
	tmp=head;
	new=(Node *)malloc(sizeof(Node));
	new->score=score;
	strcpy(new->name,tempname);

	if(head==NULL)
	{
		new->link=NULL;
		head=new;
	}
	else  //찾아서 넣어준다.
	{
		while(1)
		{
			if(tmp->link==NULL || tmp->link->score < score)
				break;
			tmp=tmp->link;
		}
		new->link=tmp->link;
		tmp->link=new;
	}
	
	user_num++;
	noecho();
	getch();
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
