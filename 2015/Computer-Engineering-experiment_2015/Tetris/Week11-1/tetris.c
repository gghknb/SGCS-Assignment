#include "tetris.h"

static struct sigaction act, oact;

#define CHILDREN_MAX 36

typedef struct _RecNode{
	int lv,score;
	char (*f)[WIDTH];
	struct _RecNode *c[CHILDREN_MAX];
} RecNode;

RecNode *recRoot;

void destructRecTree(RecNode *root);
void constructRecTree(RecNode *root);

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
//	srand(1);

	createRankList();
	/* recRoot 초기화 */
	recRoot=(RecNode *)malloc(sizeof(RecNode));
	recRoot->lv=-1;
	recRoot->score=0;
	recRoot->f=field;
	constructRecTree(recRoot);
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_R_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}
	writeRankFile();

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	for(i=0;i<VISIBLE_BLOCKS;++i){
		nextBlock[i]=rand()%7;
	}
	recommend();
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
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
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
	/* 처음 나타난 블럭을 그려준다.*/
}


void PrintScore(int score){
	move(17,WIDTH+11);
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
	}/*
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){				
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}*/
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

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j,ret=1;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				if(f[blockY+i][blockX+j]) return 0;
				if(blockY+i>=HEIGHT) return 0;
				if(blockX+j<0) return 0;
				if(blockX+j>=WIDTH) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j;
	int blk = currentBlock,rot = blockRotate, y = blockY, x = blockX;
	int oldShadowY;
	switch(command){
	case KEY_UP:
		rot=(rot+3)%4;
		break;
	case KEY_DOWN:
		y--;
		break;
	case KEY_LEFT:
		x++;
		break;
	case KEY_RIGHT:
		x--;
		break;
	}
	oldShadowY=y;
	while(CheckToMove(f,blk,rot,++oldShadowY,x));
	--oldShadowY;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blk][rot][i][j]==1){
				if(i+y>=0){ // 커맨드 입력 전 블럭의 흔적을 지운다.
					move(i+y+1,j+x+1);
					printw(".");
				}
				if(i+oldShadowY>=0){ // 커맨드 입력 전 블럭의 그림자 흔적을 지운다.
					move(i+oldShadowY+1,j+x+1);
					printw(".");
				}
			}
		}

	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
	move(HEIGHT,WIDTH+10);
}

void BlockDown(int sig){
	int drawFlag=0;
	int i;
	if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX))){
		blockY++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX);
	}
	else{
		if(blockY==-1) gameOver=1;
		score+=AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);
		score+=DeleteLine(field);
		blockY=-1;blockX=(WIDTH/2)-2;blockRotate=0;
		for(i=0;i<VISIBLE_BLOCKS-1;++i){
			nextBlock[i] = nextBlock[i+1];
		}
		nextBlock[VISIBLE_BLOCKS-1] = rand()%7;
		recommend();
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	}
	timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int i,j;
	int touched = 0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				f[blockY+i][blockX+j] = 1;
				if(blockY+i+1==HEIGHT || f[blockY+i+1][blockX+j]==1){
					++touched;
				}
			}
		}
	}
	return 10*touched;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	int i,j,k;
	int fillFlag;
	int count=0;
	for(j=1;j<HEIGHT;j++){
		fillFlag=1;
		for(i=0;i<WIDTH;i++){
			if(f[j][i]==0){
				fillFlag=0;
				break;
			}
		}
		if(fillFlag==1){
			count++;
			for(k=j;k>0;k--){
				for(i=0;i<WIDTH;i++){
					f[k][i] = f[k-1][i];
				}
			}
		}
	}
	return 100*count*count;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	while(CheckToMove(field,blockID,blockRotate,++y,x));
	DrawBlock(--y,x,blockID,blockRotate,'/');
}

void DrawRecommend(){ //recommend 된 블록을 그려준다.
	if(CheckToMove(field,nextBlock[0],recommendR,recommendY,recommendX)==1)
		DrawBlock(recommendY,recommendX,nextBlock[0],recommendR,'R');
}

void DrawBlockWithFeatures(int y, int x, int blockID,int blockRotate){
	DrawRecommend();  //recommend 된 블록을 그려준다
	DrawShadow(y,x,blockID,blockRotate);
	DrawBlock(y,x,blockID,blockRotate,' ');
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

void createRankList(){
	int i;
	Node *rankList;
	FILE *fp;
	char *ptr;

	//read rank file
	if ((fp=fopen("rank.txt","r"))==NULL){
		root=NULL;
		return;
	}
	fscanf(fp,"%d\n",&ranks);
	rankList=(Node *)malloc(sizeof(Node)*ranks);
	for(i=0;i<ranks;++i){
		fscanf(fp,"%s ",rankList[i].name);
		fscanf(fp,"%d\n",&rankList[i].score);
	}

	//construct bst from the list
	root=constructBST(rankList,0,ranks-1);
	free(rankList);
	fclose(fp);
}

Node *constructBST(Node *src,int from,int to){
	int mid=(from+to)/2;
	Node *node;
	if(from>to) return NULL;
	node=(Node *)malloc(sizeof(Node));
	*node = src[mid];
	node->p=NULL;
	node->lc=constructBST(src,from,mid-1);
	if(node->lc != NULL) node->lc->p=node;
	node->rc=constructBST(src,mid+1,to);
	if(node->rc != NULL) node->rc->p=node;
	return node;
}

typedef struct _Range{
	int from,to;
} Range;

int printNode(Node *node,void *aux){
	Range *r=aux;
	if(r->from-1 > 0){
		--r->from;
		--r->to;
		return 0;
	}
	if(r->to-1 < 0) return 0;
	--r->to;
	printw(" %-16s | %d \n",node->name,node->score);
	return 1;
}

int writeNode(Node *node,void *aux){
	fprintf(aux,"%s ",node->name,aux);
	fprintf(aux,"%d\n",node->score);
	return 1;
}

int printNodeByName(Node *node,void *aux){
	if(strcmp(aux,node->name)!=0) return 0;
	printw(" %-16s | %d \n",node->name,node->score);
	return 1;
}

int deleteNode(Node *node,void *aux){
	Node *ptr,**ptr2;
	if(--*((int *)aux) != 0){
		return 0;
	}
	if(node->p == NULL){ // the node is a root
		if(node->rc != NULL){
			root=node->rc;
			root->p=NULL;
			for(ptr=node->rc;ptr->lc != NULL;ptr=ptr->lc);
			ptr->lc=node->lc;
			if(node->lc != NULL){
				node->lc->p=ptr;
			}
		}
		else{ // has no right child
			root=node->lc;
			if(root != NULL){
				root->p=NULL;
			}
		}
	}
	else{
		if(node->p->lc == node){
			ptr2=&(node->p->lc);
		}
		else{
			ptr2=&(node->p->rc);
		}
		if(node->rc != NULL){
			*ptr2=node->rc;
			node->rc->p=node->p;
			for(ptr=node->rc;ptr->lc != NULL;ptr=ptr->lc);
			ptr->lc=node->lc;
			if(node->lc != NULL){
				node->lc->p=ptr;
			}
		}
		else{ // has no right child
			*ptr2=node->lc;
			if(node->lc != NULL){
				node->lc->p=node->p;
			}
		}
	}
	free(node);
	--ranks;
	return 1;
}

int traverseBST(Node *root,int (*applyTo)(Node *node,void *aux),void *aux){
	int ret;
	if(root==NULL) return 0;
	ret=traverseBST(root->lc,applyTo,aux);
	if(applyTo!=NULL) ret+=applyTo(root,aux);
	ret+=traverseBST(root->rc,applyTo,aux);
	return ret;
}

void rank(){
	Range r;
	char name[NAMELEN];
	int del;
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	switch(wgetch(stdscr)){
	case '1':
		echo();
		printw("X: ");
		scanw("%d",&r.from);
		printw("Y: ");
		scanw("%d",&r.to);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		if(traverseBST(root,printNode,&r)==0){
			printw("\nsearch failure: no rank in the list\n");
		}
		break;
	case '2':
		printw("input the name: ");
		echo();
		getstr(name);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		if(traverseBST(root,printNodeByName,name)==0){
			printw("\nsearch failure: no name in the list\n");
		}
		break;
	case '3':
		printw("input the rank: ");
		echo();
		scanw("%d",&del);
		if(traverseBST(root,deleteNode,&del)==0){
			printw("\nsearch failure: the rank not in the list\n");
		}
		else{
			printw("\nresult: the rank deleted\n");
		}
		noecho();
		break;
	default: break;
	}

	getch();
}

void writeRankFile(){
	FILE *fp=fopen("rank.txt","w");
	fprintf(fp,"%d\n",ranks);
	traverseBST(root,writeNode,fp);
	fclose(fp);
}

void addToBST(Node *new){
	Node *parent,*node;
	int who=0; // 0: root, 1: left child, 2: right child
	node=root;
	while(node!=NULL){
		parent=node;
		if(new->score>node->score){
			who=1;
			node=node->lc;
		}
		else{
			who=2;
			node=node->rc;
		}
	}
	switch(who){
	case 0: root=new; break;
	case 1: parent->lc=new; break;
	case 2: parent->rc=new; break;
	default: break;
	}
	new->lc=new->rc=NULL;
	++ranks;
}

void newRank(int score){
	Node *new;
	clear();
	new=(Node *)malloc(sizeof(Node));
	new->score=score;
	printw("your name: ");
	echo();
	getstr(new->name);
	noecho();
	addToBST(new);
}

void constructRecTree(RecNode *root){ //트리를 생성해주는 함수 
	int i,h;
	RecNode **c=root->c;
	for(i=0;i<CHILDREN_MAX;++i){
		c[i]=(RecNode *)malloc(sizeof(RecNode));
		c[i]->lv=root->lv+1;
		c[i]->f=(char (*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
		if(c[i]->lv < VISIBLE_BLOCKS){
			constructRecTree(c[i]);
		}
	}
}

void destructRecTree(RecNode *root){  //트리를 제거해주는 함수 
	int i,h;
	RecNode **c=root->c;
	for(i=0;i<CHILDREN_MAX;++i){
		if(c[i]->lv < VISIBLE_BLOCKS){
			destructRecTree(c[i]);
		}
		free(c[i]->f);
		free(c[i]);
	}
}

int evalState(int lv,char f[HEIGHT][WIDTH],int r,int y,int x){ //블록하나가 내려갈때 쌓여가는 점수를 리턴해주는 함수 
	return AddBlockToField(f,nextBlock[lv],r,y,x) + DeleteLine(f);
}

int recommendUsingTree(RecNode *root){ //어디에 쌓아야 점수가 잘 나는지 알수 있게 추천 해주는 함수
	int r,x,y,rBoundary,lBoundary;  //오른쪽 왼쪽 최대
	int h,w;
	int eval;
	int max=0;
	int solR,solY,solX;
	int recommended=0;
	int i=0;
	int lv=root->lv+1;
	RecNode **c=root->c;

	for(r=0;r<NUM_OF_ROTATE;++r){  //rotate수 만큼 for문을 돌려준다 /
		/* lBoundary 와 rBoundary 초기화*/
		lBoundary=3;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=0;w<BLOCK_WIDTH;++w){
				if(block[nextBlock[lv]][r][h][w]){
					break;
				}
			}
			if(w<lBoundary){
				lBoundary=w;
			}
		}
		lBoundary=0-lBoundary;

		
		rBoundary=0;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=BLOCK_WIDTH-1;w>=0;--w){
				if(block[nextBlock[lv]][r][h][w]){
					break;
				}
			}
			if(w>rBoundary){
				rBoundary=w;
			}
		}
		rBoundary=WIDTH-1-rBoundary;

		for(x=lBoundary;x<=rBoundary;++x,++i){ 
			
			for(h=0;h<HEIGHT;++h){
				for(w=0;w<WIDTH;++w){
					c[i]->f[h][w]=root->f[h][w];
				}
			}
			/* 블록을 놓을수있는지 없는지 확인해주고 새로운 필드에 블록을 놓는다. */
			y=0;
			if(CheckToMove(c[i]->f,nextBlock[lv],r,y,x)){
				while(CheckToMove(c[i]->f,nextBlock[lv],r,++y,x));
				--y;
			}
			else{ 
				continue;
			}
			
			/* 정보를 저장한다 */
			c[i]->score=root->score+evalState(lv,c[i]->f,r,y,x);
			/* 현재레벨이 최대레벨보다 작을때 현재 점수를 계산하고 reculsive 를 이용하여 저장한다. */
			if(lv < VISIBLE_BLOCKS-1){
				eval=recommendUsingTree(c[i]);
			}
			else{
				eval=c[i]->score;
			}
			/* accumulated maximum score<score 일때 축적된 최대 점수의 블럭의 정보를 업데이트한다  */
			if(max<eval){
				recommended=1;
				max=eval;
				solR=r;
				solY=y;
				solX=x;
			}
		}
	}

	/*get the information of the block with the accumulated maximum score */
	if(lv==0 && recommended){
		recommendR=solR;
		recommendY=solY;
		recommendX=solX;
	}

	return max;
}

int recommendUsingBacktracking(char fieldOri[HEIGHT][WIDTH],int lv,int score){
	char fieldNew[HEIGHT][WIDTH];
	int r,x,y,rBoundary,lBoundary;
	int i,j;
	int h,w;
	int eval;
	int max=0;
	int solR,solY,solX;
	int recommended=0;

	if(lv>VISIBLE_BLOCKS-1) return score;

	for(r=0;r<NUM_OF_ROTATE;++r){
		lBoundary=3;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=0;w<BLOCK_WIDTH;++w){
				if(block[nextBlock[lv]][r][h][w]){
					break;
				}
			}
			if(w<lBoundary){
				lBoundary=w;
			}
		}
		lBoundary=0-lBoundary;

		rBoundary=0;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=BLOCK_WIDTH-1;w>=0;--w){
				if(block[nextBlock[lv]][r][h][w]){
					break;
				}
			}
			if(w>rBoundary){
				rBoundary=w;
			}
		}
		rBoundary=WIDTH-1-rBoundary;

		for(x=lBoundary;x<=rBoundary;++x){
			for(i=0;i<HEIGHT;++i){
				for(j=0;j<WIDTH;++j){
					fieldNew[i][j]=fieldOri[i][j];
				}
			}

			y=0;
			if(CheckToMove(fieldNew,nextBlock[lv],r,y,x)){
				while(CheckToMove(fieldNew,nextBlock[lv],r,++y,x));
				--y;
			}
			else{
				continue;
			}

			eval=recommendUsingBacktracking(fieldNew,lv+1,score+evalState(lv,fieldNew,r,y,x));
			if(max<eval){
				recommended=1;
				max=eval;
				solR=r;
				solY=y;
				solX=x;
			}
		}
	}

	if(lv==0 && recommended){
		recommendR=solR;
		recommendY=solY;
		recommendX=solX;
	}

	return max;
}

int recommend(){
	int ret;

	/* 방법1: RecTre 이용 */
	ret=recommendUsingTree(recRoot);

	/* 방법2: backtracking 이용 */
	//ret=recommendUsingBacktracking(field,0,0);

	return ret;
}

void updateField(int sig){
	
}

void recommendedPlay(){
	
}
