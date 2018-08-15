#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#define Wnum1 1	  //정문
#define Wnum2 2	  //알바트로스탑
#define Wnum3 3	  //토마스 모어관
#define Wnum4 4	  //금호아시아나 바오로 경영관
#define Wnum5 5	  //마태오관
#define Wnum6 6	  //삼성 가브리엘관
#define Wnum7 7	  //예수회공동체
#define Wnum8 8	  //성이냐시오관
#define Wnum9 9	  //로욜라 도서관1
#define Wnum10 10 //엠마오관
#define Wnum11 11 //로욜라 도서관2
#define Wnum12 12 //다산관 1층
#define Wnum13 13 //최양업관
#define Wnum14 14 //하비에르관
#define Wnum15 15 //곤자가 국제학사
#define Wnum16 16 //후문
#define Wnum17 17 //곤자가 플라자
#define Wnum18 18 //떼이야르관
#define Wnum19 19 //정하상관
#define Wnum20 20 //포스코 프란치스코관
#define Wnum21 21 //리찌별관
#define Wnum22 22 //아담샬관 5층
#define Wnum23 23 //리찌과학관 5층
#define Wnum24 24 //김대건관 3층북문
#define Wnum25 25 //대운동장
#define Wnum26 26 //김대건관 3층서문
#define Wnum27 27 //본관
#define Wnum28 28 //청년광장
#define Wnum29 29 //우정관 1층
#define Wnum30 30 //체육관 앞문
#define Wnum31 31 //동문회관 4층
#define Wnum32 32 //체육관 옆문
#define Wnum33 33 //남문
#define Wnum34 34 //벨라르미노 학사
#define Wnum35 35 //리찌과학관 1층
#define Wnum36 36 //아담샬관 1층
#define Wnum37 37 //김대건관 2층
#define Wnum38 38 //메리홀
#define Wnum39 39 //다산관 지하1층
#define Wnum40 40 //우정관 2층
#define Wnum41 41 //정하상관 1층

#define MAX_VERTEX 42
#define MAX_EDGE 70

typedef struct _way{
	int start_pt;
	int stop_pt;
	double distance;
	int time;
}way;	//길의 정보를 저장할 구조체

typedef struct _building{
	int count;
	char name[40];
	char f1[40];
	char f2[40];
	char f3[40];
	char f4[40];
	char f5[40];
	char f6[40];
	char f7[40];
}building;

typedef struct _graph{
	int vertex_num;  //정점의 개수 
	int **adj_mat; //그래프의 인접행렬 이었으나 원인을 알수없게 
										//Weight_table은 값을 못받길래 이거를 가중치로 둠
	//int Weight_table[MAX_VERTEX][MAX_VERTEX];
}Graph;

Graph *map;
building b[37];
way Warr[70];	//길의 정보가 저장된 구조체의 배열 선언

void introduction();
void navigation();
void wayinit();		//길의 정보를 초기화하는 함수
char *printPlace(int wNum);	//지점의 이름을 return해주는 함수
void printWay();	//길을 출력해주는 함수
int search(char *place); //입력한 정보를 검사해서 찾아주는 함수
int insertname();
int insertfacility();
void InitGraph();
void InsertVertex();
void InsertEdge(int start, int end, int times);
void InitSogangMap();
void Findshortesttime(int cost_table[MAX_VERTEX][MAX_VERTEX]);
void findFast(int start, int end);
void FindWay(int start, int end);

int via_table[MAX_VERTEX][MAX_VERTEX] = {{0}}; // 시작시 0으로 초기화

void wayinit()
{	
	//정문에서 알바트로스탑까지
	Warr[0].start_pt=Wnum1;
	Warr[0].stop_pt=Wnum2;
	Warr[0].time=40;
	
	//정문에서 우정관 1층까지
	Warr[1].start_pt=Wnum1;
	Warr[1].stop_pt=Wnum29;
	Warr[1].time=110;

	//정문에서 삼성 가브리엘관까지
	Warr[2].start_pt=Wnum1;
	Warr[2].stop_pt=Wnum6;
	Warr[2].time=65;

	//정문에서 청년광장까지
	Warr[3].start_pt=Wnum1;
	Warr[3].stop_pt=Wnum28;
	Warr[3].time=110;
	
	//토마스 모어관에서 금호 아시아나 바오로 경영관까지 
	Warr[4].start_pt=Wnum3;
	Warr[4].stop_pt=Wnum4;
	Warr[4].time=50;
	
	//금호 아시아나 바오로 경영관에서 마태오관까지
	Warr[5].start_pt=Wnum4;
	Warr[5].stop_pt=Wnum5;
	Warr[5].time=25;
	
	//토마스 모어관에서 마태오관까지
	Warr[6].start_pt=Wnum3;
	Warr[6].stop_pt=Wnum5;
	Warr[6].time=75;
	
	//금호 아시아나 바오로 경영관에서 성이냐시오관까지
	Warr[7].start_pt=Wnum4;
	Warr[7].stop_pt=Wnum8;
	Warr[7].time=65;
		
	//성이냐시오관에서 예수회공동체까지
	Warr[8].start_pt=Wnum8;
	Warr[8].stop_pt=Wnum7;
	Warr[8].time=45;

	//예수회공동체에서 본관까지
	Warr[9].start_pt=Wnum7;
	Warr[9].stop_pt=Wnum27;
	Warr[9].time=30;

	//삼성 가브리엘관부터 예수회공동체까지
	Warr[10].start_pt=Wnum6;
	Warr[10].stop_pt=Wnum7;
	Warr[10].time=115;

	//예수회공동체에서 메리홀까지
	Warr[11].start_pt=Wnum7;
	Warr[11].stop_pt=Wnum38;
	Warr[11].time=45;
	
	//삼성 가브리엘관에서 메리홀까지
	Warr[12].start_pt=Wnum6;
	Warr[12].stop_pt=Wnum38;
	Warr[12].time=90;
	
	//본관에서 청년광장까지
	Warr[13].start_pt=Wnum27;
	Warr[13].stop_pt=Wnum28;
	Warr[13].time=68;
	
	//우정관 1층에서 체육관앞문까지
	Warr[14].start_pt=Wnum29;
	Warr[14].stop_pt=Wnum30;
	Warr[14].time=20;
	
	//체육관앞문에서 동문회관 4층까지
	Warr[15].start_pt=Wnum30;
	Warr[15].stop_pt=Wnum31;
	Warr[15].time=15;
	
	//체육관앞문에서 체육관옆문까지
	Warr[16].start_pt=Wnum30;
	Warr[16].stop_pt=Wnum32;
	Warr[16].time=35;

	//체육관옆문에서 김대건관 2층까지
	Warr[17].start_pt=Wnum32;
	Warr[17].stop_pt=Wnum37;
	Warr[17].time=10;

	//김대건관 2층에서 김대건관 3층서문까지
	Warr[18].start_pt=Wnum37;
	Warr[18].stop_pt=Wnum26;
	Warr[18].time=55;

	//남문에서 김대건관 2층까지
	Warr[19].start_pt=Wnum33;
	Warr[19].stop_pt=Wnum37;
	Warr[19].time=72;
	
	//대운동장에서 엠마오관까지
	Warr[20].start_pt=Wnum25;
	Warr[20].stop_pt=Wnum10;
	Warr[20].time=67;
	
	//대운동장에서 청년광장까지
	Warr[21].start_pt=Wnum25;
	Warr[21].stop_pt=Wnum28;
	Warr[21].time=158;
	
	//정문에서 토마스 모어관
	Warr[22].start_pt=Wnum1;
	Warr[22].stop_pt=Wnum3;
	Warr[22].time=40;
	
	//대운동장에서 로욜라 도서관1까지
	Warr[23].start_pt=Wnum25;
	Warr[23].stop_pt=Wnum9;
	Warr[23].time=148;
	
	//예수회공동체에서 로욜라 도서관1까지
	Warr[24].start_pt=Wnum7;
	Warr[24].stop_pt=Wnum9;
	Warr[24].time=85;

	//벨라르미노학사에서 리찌과학관 1층까지
	Warr[25].start_pt=Wnum34;
	Warr[25].stop_pt=Wnum35;
	Warr[25].time=70;

	//리찌과학관 1층에서 아담샬관 1층까지
	Warr[26].start_pt=Wnum35;
	Warr[26].stop_pt=Wnum36;
	Warr[26].time=60;

	//로욜라 도서관1에서 로욜라 도서관2까지
	Warr[27].start_pt=Wnum9;
	Warr[27].stop_pt=Wnum11;
	Warr[27].time=25;
	
	//로욜라 도서관1에서 최양업관까지
	Warr[28].start_pt=Wnum9;
	Warr[28].stop_pt=Wnum13;
	Warr[28].time=110;
	
	//로욜라 도서관2에서 다산관1층까지
	Warr[29].start_pt=Wnum11;
	Warr[29].stop_pt=Wnum12;
	Warr[29].time=75;
	
	//최양업관에서 하비에르관까지
	Warr[30].start_pt=Wnum13;
	Warr[30].stop_pt=Wnum14;
	Warr[30].time=40;
	
	//하비에르관에서 곤자가 플라자까지
	Warr[31].start_pt=Wnum14;
	Warr[31].stop_pt=Wnum17;
	Warr[31].time=50;
	
	//곤자가 플라자에서 곤자가 국제학사까지
	Warr[32].start_pt=Wnum17;
	Warr[32].stop_pt=Wnum15;
	Warr[32].time=45;

	//대운동장에서 김대건관 3층북문까지
	Warr[33].start_pt=Wnum25;
	Warr[33].stop_pt=Wnum24;
	Warr[33].time=28;

	//후문에서 곤자가 플라자까지
	Warr[34].start_pt=Wnum16;
	Warr[34].stop_pt=Wnum17;
	Warr[34].time=45;

	//곤자가 플라자에서 떼이야르관까지
	Warr[35].start_pt=Wnum17;
	Warr[35].stop_pt=Wnum18;
	Warr[35].time=60;
	
	//떼이야르관에서 정하상관까지
	Warr[36].start_pt=Wnum18;
	Warr[36].stop_pt=Wnum19;
	Warr[36].time=45;
	
	//정하상관에서 포스코 프란치스코관까지
	Warr[37].start_pt=Wnum19;
	Warr[37].stop_pt=Wnum20;
	Warr[37].time=20;
	
	//포스코 프란치스코관에서 리찌별관까지
	Warr[38].start_pt=Wnum20;
	Warr[38].stop_pt=Wnum21;
	Warr[38].time=55;
	
	//리찌별관에서 아담샬관 5층까지
	Warr[39].start_pt=Wnum21;
	Warr[39].stop_pt=Wnum22;
	Warr[39].time=30;
	
	//아담샬관 5층에서 리찌과학관 5층까지
	Warr[40].start_pt=Wnum22;
	Warr[40].stop_pt=Wnum23;
	Warr[40].time=45;

	//리찌과학관 5층에서 김대건관 3층북문까지
	Warr[41].start_pt=Wnum23;
	Warr[41].stop_pt=Wnum24;
	Warr[41].time=27;

	//리찌과학관 5층에서 대운동장까지
	Warr[42].start_pt=Wnum23;
	Warr[42].stop_pt=Wnum25;
	Warr[42].time=50;

	//김대건관 3층서문에서 대운동장까지
	Warr[43].start_pt=Wnum26;
	Warr[43].stop_pt=Wnum25;
	Warr[43].time=63;						
	
	//체육관앞문에서 김대건관 3층서문
	Warr[44].start_pt=Wnum30;
	Warr[44].stop_pt=Wnum26;
	Warr[44].time=73;					

	//곤자가 국제학사에서 후문까지
	Warr[45].start_pt=Wnum15;
	Warr[45].stop_pt=Wnum16;
	Warr[45].time=45;

	//다산관 지하1층에서 하비에르관까지
	Warr[46].start_pt=Wnum39;
	Warr[46].stop_pt=Wnum14;
	Warr[46].time=95;

	//우정관 2층에서 김대건관 3층서문까지
	Warr[47].start_pt=Wnum40;
	Warr[47].stop_pt=Wnum26;
	Warr[47].time=86;						

	//우정관 2층에서 체육관 앞문까지
	Warr[48].start_pt=Wnum40;
	Warr[48].stop_pt=Wnum30;
	Warr[48].time=47;					
	
	//우정관 2층에서 체육관 옆문까지
	Warr[49].start_pt=Wnum40;
	Warr[49].stop_pt=Wnum32;
	Warr[49].time=55;

	//우정관 2층에서 청년광장까지
	Warr[50].start_pt=Wnum40;
	Warr[50].stop_pt=Wnum28;
	Warr[50].time=49;

	//정문에서 남문까지
	Warr[51].start_pt=Wnum1;
	Warr[51].stop_pt=Wnum33;
	Warr[51].time=170;

	//남문에서 벨라르미노 학사까지
	Warr[52].start_pt=Wnum33;
	Warr[52].stop_pt=Wnum34;
	Warr[52].time=75;

	//리찌과학관 1층에서 리찌과학관 5층에서
	Warr[53].start_pt=Wnum35;
	Warr[53].stop_pt=Wnum23;
	Warr[53].time=110;

	//아담샬관 1층에서 아담샬관 5층까지
	Warr[54].start_pt=Wnum36;
	Warr[54].stop_pt=Wnum22;
	Warr[54].time=125;

	//아담샬관 1층에서 정하상관 1층까지
	Warr[55].start_pt=Wnum36;
	Warr[55].stop_pt=Wnum41;
	Warr[55].time=90;

	//정하상관 1층에서 떼이야르관까지
	Warr[56].start_pt=Wnum41;
	Warr[56].stop_pt=Wnum18;
	Warr[56].time=50;

	//본관에서 삼성 가브리엘관까지
	Warr[57].start_pt=Wnum27;
	Warr[57].stop_pt=Wnum6;
	Warr[57].time=100;

	//곤자가 국제학사에서 하비에르관까지
	Warr[58].start_pt=Wnum15;
	Warr[58].stop_pt=Wnum14;
	Warr[58].time=45;

	//떼이야르관에서 후문까지
	Warr[59].start_pt=Wnum18;
	Warr[59].stop_pt=Wnum16;
	Warr[59].time=20;

	//후문에서 정하상관 1층까지
	Warr[60].start_pt=Wnum16;
	Warr[60].stop_pt=Wnum41;
	Warr[60].time=35;

	//동문회관 4층에서 남문까지
	Warr[61].start_pt=Wnum31;
	Warr[61].stop_pt=Wnum33;
	Warr[61].time=80;

	//로욜라 도서관1에서 엠마오관까지
	Warr[62].start_pt=Wnum9;
	Warr[62].stop_pt=Wnum10;
	Warr[62].time=75;

	//성이냐시오관에서 마태오관까지
	Warr[63].start_pt=Wnum8;
	Warr[63].stop_pt=Wnum5;
	Warr[63].time=65;

	//우정관 1층에서 우정관 2층까지
	Warr[64].start_pt=Wnum29;
	Warr[64].stop_pt=Wnum40;
	Warr[64].time=40;

	//예수회 공동체에서 대운동장까지
	Warr[65].start_pt=Wnum7;
	Warr[65].stop_pt=Wnum25;
	Warr[65].time=160;

	//본관에서 대운동장까지
	Warr[66].start_pt=Wnum27;
	Warr[66].stop_pt=Wnum25;
	Warr[66].time=150;

	//대운동장에서 리찌과학관 5층까지
	Warr[67].start_pt=Wnum25;
	Warr[67].stop_pt=Wnum23;
	Warr[67].time=45;

	//포스코 프란치스코관에서 최양업관까지
	Warr[68].start_pt=Wnum20;
	Warr[68].stop_pt=Wnum13;
	Warr[68].time=81;

	//김대건관 3층서문에서 김대건관 3층북문까지
	Warr[69].start_pt=Wnum26;
	Warr[69].stop_pt=Wnum24;
	Warr[69].time=85;
}
