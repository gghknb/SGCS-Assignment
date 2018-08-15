#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int dwall,rwall;//0없음 1있음
	int num;
}wall;

int main()
{
	int col; // 가로
	int row; // 세로
	wall **maze;
	int cnt=1;
	int i,j,k;
	int tmp;
	int flag=1;
	FILE *fp;
	fp=fopen("maze.maz","w");
	srand(time(NULL));
	scanf("%d %d",&col,&row);
	/* malloc */
	maze=(wall **)malloc(sizeof(wall*)*row);
	for(i=0;i<row;i++)
		*(maze+i)=(wall *)malloc(sizeof(wall)*col);
	
	/* initialize */
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			maze[i][j].num=cnt;
			cnt++;
			maze[i][j].dwall = 1;
			maze[i][j].rwall = 1;
		}
	}
	
	for(i=0;i<row;i++)
	{
		/* 오른쪽벽 */
		for(j=0;j<col;j++)
		{
			
				maze[i][j].rwall = rand()%2;
			
			if(i == row-1)  //맨아래줄인데 옆방이랑번호다르면 무조건 벽없애기
			{
				if(maze[i][j].num != maze[i][j+1].num && j!=col-1)
					maze[i][j].rwall = 0 ;
			}
			if(maze[i][j].rwall == 0)//현재방이랑 옆방 번호다른데 벽이없으면 for문으로 옆방들 다 현재방번호로 바꿔주기 
			{
				
				tmp = maze[i][j+1].num;
				if(j!=col-1)
					maze[i][j+1].num = maze[i][j].num;
				
				for(k=0;k<col;k++)
				{
					if(tmp==maze[i][k].num)
					{
						maze[i][k].num = maze[i][j].num;
					}
				}
			}
			if(j==col-1) maze[i][j].rwall = 1;
		}
		
		/* 아래쪽 벽 */
		for(j=0;j<col;j++)
		{
			if(i == row-1) break;
			
			maze[i][j].dwall = rand()%2;
			if(maze[i][j].dwall == 0)
				flag=0;

			if(maze[i][j].num != maze[i][j+1].num || j!=col-1)
			{
				if(flag==1)
					maze[i][j].dwall = 0;
				else if(flag==0)
					flag=1;
			}
			

		}
	}
		

	

	/*print maze */
	
	for(i=0;i<row;i++)
	{
		if(i==0) //첫줄
		{
			for(j=0;j<col;j++)
				fprintf(fp,"+-");
			fprintf(fp,"+\n");
		}
		else 
		{
			for(j=0;j<col;j++)
			{
				fprintf(fp,"+");
				if(maze[i-1][j].dwall == 1)			
					fprintf(fp,"-");
				else fprintf(fp," ");
				
			}
				fprintf(fp,"+\n");
		}

		fprintf(fp,"|");
		for(j=0;j<col;j++) 
		{
			if(maze[i][j].rwall == 1)
				fprintf(fp," |");
			else
				fprintf(fp,"  ");
		}
		fprintf(fp,"\n");
		
	}
	for(j=0;j<col;j++)
			fprintf(fp,"+-");
		fprintf(fp,"+\n");
	fclose(fp);
	return 0;
}
