#include <stdlib.h>
#include <string.h>
#include "..\ExternDoc.h"
#include "..\UI\Zoom.h"
#include "..\UI\MsgView.h"
#include "..\Graphics\DrawFunc.h"
#include "..\Example\Example.h"

#define ERROR_NUMBER -1
typedef struct{
	int row;
	int col;
	int rflag;
	int dflag;

}maze;
	//function prototype
maze **map;
int width,height;
static void drawDirect(CDC *pDC);
static void drawBuffered();

//Start of user code
#include <float.h>

/*****************************************************************
* function	: bool readFile(const char* filename)
* argument	: cons char* filename - filename to be opened
* return	: true if success, otherwise flase
* remark	: After read data file, phisycal view must be set;
*			  otherwise drawing will not be executed correctly.
*			  The window will be invalidated after readFile()
*			  returns true.
******************************************************************/

bool readFile(const char* filename){
	
	FILE *fp;
	int i,j;
	char *str;
	int cnt ;
	char str1,str2;
	str=(char*)malloc(sizeof(char)*100);
	
	fp=fopen(filename,"r");
	if(fp == NULL) return false;
	fscanf(fp,"%s",str);
	width =strlen(str);
	fseek(fp, 0, SEEK_END);  //처음 포인터로
	cnt = ftell(fp);
	
	height = cnt/(width+1);

	fseek(fp, width+1, SEEK_SET); //다음줄로
	height = (height-1)/2; 
	width = (width-1)/ 2;
	map = (maze **)malloc(sizeof(maze*)*height);
	for(i=0;i<height;i++)
		map[i] = (maze*)malloc(sizeof(maze)*width);
	fgetc(fp); //엔터
	for(i=0;i<height;i++)
	{
		str1=fgetc(fp); // |  
		for(j=0;j<width;j++)
		{
			str1=fgetc(fp);  // blank 
			str2=fgetc(fp);  //  |
			map[i][j].row = i;
			map[i][j].col = j;
			if(str2 == '|')
				map[i][j].rflag = 1; //오른벽이 있을때 
			else map[i][j].rflag = 0; // 없을때
		}
		str1 = fgetc(fp); // 엔터
		str2 = fgetc(fp); // +
		for(j=0;j<width;j++)
		{
			str1 = fgetc(fp);  //-
			str2 = fgetc(fp); // +
			if(str1 == '-')
				map[i][j].dflag = 1; //아래벽이 있을때
			else map[i][j].dflag =0; //없을때
		}
		str1 = fgetc(fp); //엔터
	}
	
	fclose(fp);
	setWindow(0,0,5*width+5,5*height+5,1);
	//start of the user code
	
	return true; //edit after finish this function
	//end of usercode
}

/******************************************************************
* function	: bool FreeMemory()
*
* remark	: Save user data to a file
*******************************************************************/
void freeMemory(){
	int i;
	for(i=0;i<height;i++)
		free(map[i]);
	free(map);
	//start of the user code
	//end of usercode
}

/**************************************************************
* function	: bool writeFile(const char* filename)
*
* argument	: const char* filename - filename to be written
* return	: true if success, otherwise false
* remark	: Save user data to a file
****************************************************************/
bool writeFile(const char* filename){
	//start of the user code
	bool flag;
	flag = 0;

	return flag;
	//end of usercode
}

/************************************************************************
* fucntion	: void drawMain(CDC* pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Main drawing function. Called by CMFC_MainView::OnDraw()
*************************************************************************/
void drawMain(CDC *pDC){
	//if direct drawing is defined
#if defined(GRAPHICS_DIRECT)
	drawDirect(pDC);
	//if buffered drawing is defined
#elif defined(GRAPHICS_BUFFERED)
	drawBuffered();
#endif
}

/************************************************************************
* function	: static void drawDirect(CDC *pDC
*
* argument	: CDC* pDC - device context object pointer
* remark	: Direct drawing routines here.
*************************************************************************/
static void drawDirect(CDC *pDC){
	//begin of user code
	//Nothing to write currently.
	//end of user code
}

/***********************************************************************
* function	: static void drawBuffered()
*
* argument	: CDC* pDC -0 device object pointer
* remark	: Buffered drawing routines here.
************************************************************************/
static void drawBuffered(){
//	VLL *ptr;
	//VTX *V;
	int lineWidth = 0.01;
	int i,j;
//	int x,y;
	DrawSolidBox_I(0,4*height,1,4*height+1,lineWidth,RGB(0,0,255),RGB(0,0,255)); //가장왼쪽위 모서리 
	for(i=0;i<width;i++) //윗변
	{
		DrawSolidBox_I(4*i+1,4*height,4*i+4,4*height+1,lineWidth,RGB(0,0,255),RGB(0,0,255));
		DrawSolidBox_I(4*i+4,4*height,4*i+5,4*height+1,lineWidth,RGB(0,0,255),RGB(0,0,255));
	}
	for(i=0;i<height;i++)
	{
		DrawSolidBox_I(0,4*(height-i)-3,1,4*(height-i),lineWidth,RGB(0,0,255),RGB(0,0,255));
		DrawSolidBox_I(0,4*(height-i-1),1,4*(height-i)-3,lineWidth,RGB(0,0,255),RGB(0,0,255));
		for(j=0;j<width;j++)
		{
			if(map[i][j].rflag == 1)
				DrawSolidBox_I(4*(j+1),4*(height-i)-3,4*j+5,4*(height-i),lineWidth,RGB(0,0,255),RGB(0,0,255));
			if(map[i][j].dflag == 1)
				DrawSolidBox_I(4*j+1,4*(height-i-1),4*(j+1),4*(height-i)-3,lineWidth,RGB(0,0,255),RGB(0,0,255));
			DrawSolidBox_I(4*(j+1),4*(height-i-1),4*j+5,4*(height-i)-3,lineWidth,RGB(0,0,255),RGB(0,0,255));
		}
	}
	//start of the user code
	//end of the user code
}