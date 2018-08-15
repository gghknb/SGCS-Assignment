#include "sni1.h"

int main()
{
	char menu=0;

	system("clear");
	while((menu-'0')!=3){ //3이 입력될 때까지 while문이 돌아감	
		printf("\n	Sogang Navigation & Introduction\n\n");
		printf("1. Search the facility\n");
		printf("2. Search the way\n");
		printf("3. Exit\n");
		menu=getchar(); //입력받은 값을 menu에 저장

		system("clear");
		switch(menu-'0'){
			case 1: introduction(); break; //menu가 1일 경우 introduction() 호출
			case 2: navigation(); break; //menu가 2일 경우 navigation() 호출
			case 3: break; //menu가 3일 경우 프로그램 종료
			default: printf("Please input the number from 1 to 3\n"); //입력된 값이 1,2,3 이외의 값일 경우 에러메시지 출력
					 getchar();
					 break;
		}
	}

	return 0;
}

void introduction() //시설과 빌딩을 소개
{
	int count = 0;
	char facilitytmp[40];
	char buildingtmp[40];
	int i, buildingcount = 0;
	int option=0;
	insertname();
	insertfacility();

	while(option!=3){
		system("clear");
		printf("Please select the option you want\n");
		printf("1. Find the facility\n");
		printf("2. Show me the facility list of the building\n");
		printf("3. Return menu\n");
		scanf("%d", &option); //원하는 option값을 입력받음
		getchar();

		switch (option){
			case 1: //option이 1일 경우
				system("clear");
				printf("Facility List\n"); //시설 리스트를 출력
				printf("----------------------------------------\n");
				printf(" 1. restaurant\n");
				printf(" 2. lounge\n");
				printf(" 3. computer labs\n");
				printf(" 4. copy room\n");
				printf(" 5. automaic copy machine\n");
				printf(" 6. library\n");
				printf(" 7. studying room\n");
				printf(" 8. female lounge\n");
				printf(" 9. center for disability service\n");
				printf("10. souvenir shop\n");
				printf("11. post office\n");
				printf("12. bank\n");
				printf("13. bank ATM\n");
				printf("14. convenient store\n");
				printf("15. bookstore\n");
				printf("16. cafe\n");
				printf("----------------------------------------\n\n");
				printf("Please enter the name of the facility: ");
				gets(facilitytmp); //사용자로부터 찾고 싶은 facility 이름을 입력받아 facilitytmp에 저장

				for (i = 0; i<37; i++){ //strcmp를 이용하여 비교해서 일치하면 출력하고 count를 1올려줌
					if (strcmp(b[i].f1, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f2, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f3, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f4, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f5, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f6, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
					if (strcmp(b[i].f7, facilitytmp) == 0){
						printf("%s\n", b[i].name);
						count++;
					}
				}
				if (count == 0) //count가 0일 경우 메시지 출력
					printf("There is no %s in Sogang University\n", facilitytmp);
				count = 0; //count를 0으로 초기화
				getchar();
				break;
			case 2: //option이 2일 경우
				system("clear");
				printf("Building list\n"); //빌딩 리스트를 출력
				printf("----------------------------------------\n");
				for (i = 0; i<37; i++){
					printf("%d. %s\n", i + 1, b[i].name);
				}
				printf("----------------------------------------\n\n");
				printf("Please enter the name of the building: ");
				gets(buildingtmp); //사용자로부터 알고자 하는 building을 입력받아 buildingtmp에 저장

				for (i = 0; i<37; i++){ //입력받은 빌딩에 존재하는 시설을 모두 출력
					if (strcmp(b[i].name, buildingtmp) == 0 && b[i].count != 0){
						printf("The %s has ", b[i].name);
						printf("%s\n", b[i].f1);
						printf("%s\n", b[i].f2);
						printf("%s\n", b[i].f3);
						printf("%s\n", b[i].f4);
						printf("%s\n", b[i].f5);
						printf("%s\n", b[i].f6);
						printf("%s\n", b[i].f7);
						buildingcount++;
					}
					else if (strcmp(b[i].name, buildingtmp) == 0 && b[i].count == 0){ //빌딩에 시설이 없을 경우
						printf("The %s has no facilities at all\n", b[i].name); //에러메시지 출력
						buildingcount++;
					}
				}
				if (buildingcount == 0) //입력받은 값의 빌딩이 존재하지 않을 경우
					printf("Please input the exact name of the building!\n");
				buildingcount = 0;
				printf("\n");
				getchar();
				break;
			case 3: //option이 3일 경우 menu로 돌아감
				system("clear");
				return;
			default://입력받은 option이 1,2,3 이외의 값일 경우 에러메시지 출력
				printf("Please input available option!\n");
				getchar();
				system("clear");
				break;
				printf("\n");
		}
	}

	return;
}

void navigation() //최단 시간 경로를 찾아서 출력
{
	int option=0, i;
	char *start, *stop;
	int start_pt, stop_pt;

	while(option!=3){
		start=(char *)malloc(sizeof(char)*40);
		stop=(char *)malloc(sizeof(char)*40);
		
		system("clear");
		printf("Please select the option you want\n");
		printf("1. Find thd way\n");
		printf("2. Return menu\n");
		scanf("%d", &option); //원하는 option값을 입력받음
		getchar();

		switch(option){
		case 1: //option값이 1일 경우
			system("clear");
			printf("Location list\n"); //지점 리스트를 출력
			printf("----------------------------------------\n");
			for(i=1; i<=40; i++){
				printf("%2d. %s\n",i,printPlace(i));
			}
			printf("\n");
			printf("Input start point : ");
			gets(start); //start point를 입력받음
			printf("Input destination point  : ");
			gets(stop); //stop point를 입력받음
			
			start_pt=search(start); //search() 함수를 이용해 입력받은 지점의 정보를 찾아서 return
			stop_pt=search(stop); //search() 함수를 이용해 입력받은 지점의 정보를 찾아서 return

			if(start_pt==0 || stop_pt==0){ //둘 중 하나라도 저장된 정보와 일치하지 않을 경우 에러메시지 출력
				printf("Wrong Input!\n");
				getchar();
				break;
			}
			else
				FindWay(start_pt, stop_pt);
			getchar();
			break;
		case 2: //option값이 2일 경우
			system("clear");
			return; //menu로 돌아감
		}
		free(start); //할당된 메모리 free
		free(stop); //할당된 메모리 free
	}

	return;
}

int insertname(){ //빌딩의 이름을 배열에 저장
	int i;
	for (i = 0; i<37; i++)
		b[i].count = 0;
	strcpy(b[0].name, "Main Gate");
	strcpy(b[1].name, "Albatross Tower");
	strcpy(b[2].name, "Administration Bldg");
	strcpy(b[3].name, "Student Union Bldg");
	strcpy(b[4].name, "Jesuit Community");
	strcpy(b[5].name, "Samsung Gabriel Hall");
	strcpy(b[6].name, "Kumho Asiana Paulus Hall");
	strcpy(b[7].name, "Thomas More Hall");
	strcpy(b[8].name, "Matthew Hall");
	strcpy(b[9].name, "Mary Hall");
	strcpy(b[10].name, "Saint Ignatius House");
	strcpy(b[11].name, "Loyola Garden");
	strcpy(b[12].name, "Emmaus Hall");
	strcpy(b[13].name, "Loyola Library");
	strcpy(b[14].name, "Choe Yangeop Hall");
	strcpy(b[15].name, "Xavier Hall");
	strcpy(b[16].name, "Dasan Hall");
	strcpy(b[17].name, "Gonzaga Hall");
	strcpy(b[18].name, "Back Gate");
	strcpy(b[19].name, "Gonzaga Plaza");
	strcpy(b[20].name, "Teilhard Hall");
	strcpy(b[21].name, "Jeong Hasang Hall");
	strcpy(b[22].name, "POSCO Francisco Hall");
	strcpy(b[23].name, "Ricci Hall Annex");
	strcpy(b[24].name, "Sports Ground");
	strcpy(b[25].name, "Adam Schall Hall");
	strcpy(b[26].name, "Ricci Hall");
	strcpy(b[27].name, "Jesuit Apostolic Center");
	strcpy(b[28].name, "Kim Daegon Hall");
	strcpy(b[29].name, "Bellarmino Dormitory");
	strcpy(b[30].name, "Sogang Bldg");
	strcpy(b[31].name, "South Gate");
	strcpy(b[32].name, "Arrupe Hall");
	strcpy(b[33].name, "Gymnasium");
	strcpy(b[34].name, "Youth Plaza");
	strcpy(b[35].name, "Berchmans Woojung Hall");
	strcpy(b[36].name, "Parking Lot");

	return 0;
}

int insertfacility(){ //지점의 정보를 배열에 저장
	strcpy(b[35].f1, "restaurant");
	b[35].count++;
	strcpy(b[12].f1, "restaurant");
	b[12].count++;
	strcpy(b[19].f1, "restaurant");
	b[19].count++;

	strcpy(b[13].f1, "lounge");
	b[13].count++;
	strcpy(b[8].f1, "lounge");
	b[8].count++;
	strcpy(b[35].f2, "lounge");
	b[35].count++;

	strcpy(b[25].f1, "computer labs");
	b[25].count++;
	strcpy(b[21].f1, "computer labs");
	b[21].count++;
	strcpy(b[28].f1, "computer labs");
	b[28].count++;
	strcpy(b[15].f1, "computer labs");
	b[15].count++;
	strcpy(b[5].f1, "computer labs");
	b[5].count++;
	strcpy(b[8].f2, "computer labs");
	b[8].count++;

	strcpy(b[13].f2, "copy room");
	b[13].count++;
	strcpy(b[5].f1, "copy room");
	b[5].count++;
	strcpy(b[8].f3, "copy room");
	b[8].count++;
	strcpy(b[28].f2, "copy room");
	b[28].count++;
	strcpy(b[26].f1, "copy room");
	b[26].count++;
	strcpy(b[21].f2, "copy room");
	b[21].count++;
	strcpy(b[16].f1, "copy room");
	b[16].count++;

	strcpy(b[13].f3, "automatic copy machine");
	b[13].count++;
	strcpy(b[8].f4, "automatic copy machine");
	b[8].count++;
	strcpy(b[15].f2, "automatic copy machine");
	b[15].count++;

	strcpy(b[13].f4, "library");
	b[13].count++;
	strcpy(b[8].f4, "library");
	b[8].count++;

	strcpy(b[28].f3, "studying room");
	b[28].count++;
	strcpy(b[13].f5, "studying room");
	b[13].count++;
	strcpy(b[15].f3, "studying room");
	b[15].count++;

	strcpy(b[35].f3, "female lounge");
	b[35].count++;
	strcpy(b[15].f4, "female lounge");
	b[15].count++;

	strcpy(b[35].f4, "center for disability service");
	b[35].count++;

	strcpy(b[35].f5, "souvenir shop");
	b[35].count++;

	strcpy(b[35].f6, "post office");
	b[35].count++;

	strcpy(b[32].f1, "bank");
	b[32].count++;

	strcpy(b[3].f1, "bank ATM");
	b[3].count++;

	strcpy(b[32].f2, "convenient store");
	b[32].count++;
	strcpy(b[17].f1, "convenient store");
	b[17].count++;

	strcpy(b[35].f7, "cafe");
	b[35].count++;
	strcpy(b[19].f2, "cafe");
	b[19].count++;

	strcpy(b[19].f3, "bookstore");
	b[19].count++;

	return 0;
}

char *printPlace(int Wnum) //지점의 이름을 return해주는 함수
{
	switch (Wnum){
	case Wnum1:
		return "Main Gate"; break;
	case Wnum2:
		return "Albatross Tower"; break;
	case Wnum3:
		return "Thomas More Hall"; break;
	case Wnum4:
		return "Kumho Asiana Paulus Hall"; break;
	case Wnum5:
		return "Matthew Hall"; break;
	case Wnum6:
		return "Samsung Gabriel Hall"; break;
	case Wnum7:
		return "Jesuit Community"; break;
	case Wnum8:
		return "Saint Ignatius House"; break;
	case Wnum9:
		return "Loyola Library1"; break;
	case Wnum10:
		return "Emmaus Hall"; break;
	case Wnum11:
		return "Loyola Library2"; break;
	case Wnum12:
		return "Dasan Hall 1st Floor"; break;
	case Wnum13:
		return "Choe Yangeop Hall"; break;
	case Wnum14:
		return "Xavier Hall"; break;
	case Wnum15:
		return "Gonzaga Hall"; break;
	case Wnum16:
		return "Back Gate"; break;
	case Wnum17:
		return "Gonzaga Plaza"; break;
	case Wnum18:
		return "Teilhard Hall"; break;
	case Wnum19:
		return "Jeong Hasang Hall 4th Floor"; break;
	case Wnum20:
		return "POSCO Francisco Hall"; break;
	case Wnum21:
		return "Ricci Hall Annex"; break;
	case Wnum22:
		return "Adam Schall Hall 5th Floor"; break;
	case Wnum23:
		return "Ricci Hall 5th Floor"; break;
	case Wnum24:
		return "Kim Daegon Hall North Gate"; break;
	case Wnum25:
		return "Sports Ground"; break;
	case Wnum26:
		return "Kim Daegon Hall West Gate"; break;
	case Wnum27:
		return "Administration Bldg"; break;
	case Wnum28:
		return "Youth Plaza"; break;
	case Wnum29:
		return "Berchmans Woojung Hall 1st Floor"; break;
	case Wnum30:
		return "Gymnasium Front Door"; break;
	case Wnum31:
		return "Arrupe Hall 4th Floor"; break;
	case Wnum32:
		return "Gymnasium Side Door"; break;
	case Wnum33:
		return "South Gate"; break;
	case Wnum34:
		return "Bellarmino Dormitory"; break;
	case Wnum35:
		return "Ricci Hall 1st Floor"; break;
	case Wnum36:
		return "Adam Schall Hall 1st Floor"; break;
	case Wnum37:
		return "Kim Daegon Hall 2nd Floor"; break;
	case Wnum38:
		return "Mary Hall"; break;
	case Wnum39:
		return "Dasan Hall B1"; break;
	case Wnum40:
		return "Berchmans Woojung Hall 2nd Floor"; break;
	case Wnum41:
		return "Jeong Hasang Hall 1st Floor"; break;
	default:
		return "Complete"; break;
	}
}

int search(char *place) //입력한 정보를 검사해서 찾아주는 함수
{
	if(strcmp(place, "정문")==0 || strcmp(place,"Main Gate")==0 ||strcmp(place, "1")==0)
		return Wnum1;
	else if(strcmp(place, "알바트로스탑")==0 || strcmp(place, "알바탑")==0 || strcmp(place, "탑")==0 ||strcmp(place,"Albatross Tower")==0 || strcmp(place, "2")==0)
		return Wnum2;
	else if(strcmp(place, "토마스 모어관")==0 || strcmp(place, "토마스관")==0 || strcmp(place, "토마스모어관")==0 || strcmp(place, "T관")==0 || strcmp(place, "t관")==0 || strcmp(place, "티관")==0 ||strcmp(place,"Thomas More Hall")==0 ||strcmp(place, "3")==0)
		return Wnum3;
	else if(strcmp(place, "금호아시아나 바오로 경영관")==0 || strcmp(place, "금호아시아나")==0 || strcmp(place, "금호아시아나관")==0 || strcmp(place, "바오로 경영관")==0 || strcmp(place, "PA관")==0 || strcmp(place, "pa관")==0 || strcmp(place, "Pa관")==0 ||strcmp(place,"Kumho Asiana Paulus Hall")==0 ||strcmp(place, "4")==0)
		return Wnum4;
	else if(strcmp(place, "마태오관")==0 || strcmp(place, "마태오")==0 ||strcmp(place, "MA관")==0 || strcmp(place, "Ma관")==0 || strcmp(place, "ma관")==0 ||strcmp(place,"Matthew Hall")==0 ||strcmp(place, "5")==0)
		return Wnum5;
	else if(strcmp(place, "삼성 가브리엘관")==0 || strcmp(place, "가브리엘관")==0 || strcmp(place, "Ga관")==0 || strcmp(place, "ga관")==0 || strcmp(place, "GA관")==0 ||strcmp(place,"Samsung Gabriel Hall")==0 ||strcmp(place, "6")==0)
		return Wnum6;
	else if(strcmp(place, "예수회공동체")==0 || strcmp(place, "예수회공동회")==0 || strcmp(place, "예수회")==0 ||strcmp(place,"Jesuit Community")==0 ||strcmp(place, "7")==0)
		return Wnum7;
	else if(strcmp(place, "성이냐시오관")==0 || strcmp(place, "이냐시오관")==0 || strcmp(place, "이냐시오")==0 || strcmp(place, "I관")==0 || strcmp(place, "i관")==0 ||strcmp(place,"Saint Ignatius House")==0 || strcmp(place, "8")==0)
		return Wnum8;
	else if(strcmp(place, "로욜라 도서관 1관")==0 || strcmp(place, "로욜라 도서관")==0 || strcmp(place, "로욜라")==0 ||strcmp(place, "로욜라도서관")==0 ||strcmp(place,"Loyola Library")==0 || strcmp(place, "Loyola Library1")==0 || strcmp(place, "9")==0)	
		return Wnum9;
	else if(strcmp(place, "엠마오관")==0 || strcmp(place, "엠마오")==0 || strcmp(place, "E관")==0 || strcmp(place, "e관")==0 ||strcmp(place,"Emmaus Hall")==0 ||strcmp(place, "10")==0)
		return Wnum10;
	else if(strcmp(place, "로욜라 도서관 2관")==0 ||strcmp(place,"Loyola Library2")==0 || strcmp(place, "11")==0)
		return Wnum11;
	else if(strcmp(place, "다산관 1층")==0 || strcmp(place, "다산관")==0 || strcmp(place, "D관")==0 || strcmp(place, "d관")==0 ||strcmp(place,"Dasan Hall")==0 || strcmp(place, "Dasan Hall 1st Floor")==0 || strcmp(place, "12")==0)
		return Wnum12;
	else if(strcmp(place, "최양업관")==0 || strcmp(place, "최양업")==0 || strcmp(place, "CY관")==0 || strcmp(place, "cy관")==0 || strcmp(place, "Cy관")==0 ||strcmp(place,"Choe Yangeop Hall")==0|| strcmp(place, "13")==0)
		return Wnum13;
	else if(strcmp(place, "하비에르관")==0 || strcmp(place, "하비에르")==0 || strcmp(place, "X관")==0 || strcmp(place, "x관")==0 ||strcmp(place,"Xavier Hall")==0 ||strcmp(place, "14")==0)
		return Wnum14;
	else if(strcmp(place, "곤자가 국제학사")==0 || strcmp(place, "국제학사")==0 || strcmp(place, "곤자가 기숙사")==0 || strcmp(place, "GH")==0 || strcmp(place, "gh")==0 || strcmp(place, "Gh")==0 ||strcmp(place,"Gonzaga Hall")==0 ||strcmp(place, "15")==0)
		return Wnum15;
	else if(strcmp(place, "후문")==0 ||strcmp(place,"Back Gate")==0 ||strcmp(place, "16")==0)
		return Wnum16;
	else if(strcmp(place, "곤자가 플라자")==0 || strcmp(place, "곤플")==0 || strcmp(place, "GP")==0 || strcmp(place, "Gp")==0 || strcmp(place, "gp")==0 ||strcmp(place,"Gonzaga Plaza")==0 ||strcmp(place, "17")==0)
		return Wnum17;
	else if(strcmp(place, "떼이야르관")==0 || strcmp(place, "떼이아르관")==0 || strcmp(place, "떼이야르")==0 || strcmp(place, "TE관")==0 || strcmp(place, "Te관")==0 || strcmp(place, "te관")==0 ||strcmp(place,"Teilhard Hall")==0 ||strcmp(place, "18")==0)
		return Wnum18;
	else if(strcmp(place, "정하상관")==0 || strcmp(place, "J관")==0 || strcmp(place, "j관")==0 || strcmp(place, "제이관")==0 ||strcmp(place,"Jeong Hasang Hall")==0 || strcmp(place, "정하상관 5층")==0 || strcmp(place, "Jeong Hasang Hall 4th Floor")==0 ||strcmp(place, "19")==0)
		return Wnum19;
	else if(strcmp(place, "포스코 프란치스코관")==0 || strcmp(place, "포스코")==0 || strcmp(place, "포스코관")==0 || strcmp(place, "프란치스코관")==0 || strcmp(place, "F관")==0 || strcmp(place, "p관")==0 ||strcmp(place,"POSCO Francisco Hall")==0 || strcmp(place, "20")==0)
		return Wnum20;
	else if(strcmp(place, "리찌별관")==0 || strcmp(place, "알에이관")==0 || strcmp(place, "RA관")==0 || strcmp(place, "ra관")==0 || strcmp(place, "Ra관")==0 ||strcmp(place,"Ricci Hall Annex")==0 ||strcmp(place, "21")==0)
		return Wnum21;
	else if(strcmp(place, "아담샬관")==0 || strcmp(place, "에이에스관")==0 || strcmp(place, "AS관")==0 || strcmp(place, "As관")==0 || strcmp(place, "as관")==0 || strcmp(place, "AS관 5층")==0 || strcmp(place, "As관 5층") == 0 || strcmp(place, "as관 5층") == 0 ||strcmp(place,"Adam Schall Hall")==0 || strcmp(place, "Adam Schall Hall 5th Floor")==0 ||strcmp(place, "22")==0)
		return Wnum22;
		else if(strcmp(place, "리찌과학관")==0 || strcmp(place, "알관")==0 || strcmp(place, "알오")==0 || strcmp(place, "r관")==0 || strcmp(place, "R관")==0 || strcmp(place, "리찌과학관 5층") == 0 ||strcmp(place,"Ricci Hall")==0 ||strcmp(place, "Ricci Hall 5th Floor")==0 ||strcmp(place, "23")==0)
		return Wnum23;	
	else if(strcmp(place, "김대건관 3층북문")==0 || strcmp(place, "김대건관 북문")==0 || strcmp(place, "케이관")==0 || strcmp(place, "K관")==0 || strcmp(place, "k관")==0 || strcmp(place, "김대건관 3층")==0 || strcmp(place, "김대건관") == 0 ||strcmp(place,"Kim Daegon Hall West Gate")==0 ||strcmp(place, "24")==0)
		return Wnum24;	
	else if(strcmp(place, "대운동장")==0 ||strcmp(place,"Sports Ground")==0 ||strcmp(place, "25")==0)
		return Wnum25;
	else if(strcmp(place, "김대건관 3층서문")==0 || strcmp(place, "김대건관 서문")==0 || strcmp(place, "K관")==0 || strcmp(place, "k관")==0 || strcmp(place, "김대건관 3층")==0 ||strcmp(place,"Kim Daegon Hall 3rd Floor")==0 ||strcmp(place,"Kim Daegon Hall South Gate")==0 ||strcmp(place, "26")==0) 
		return Wnum26;
	else if(strcmp(place, "본관")==0 ||strcmp(place,"Administration Bldg")==0 || strcmp(place, "27")==0)
		return Wnum27;
	else if(strcmp(place, "청년광장")==0 || strcmp(place, "청광")==0 ||strcmp(place,"Youth Plaza")==0 || strcmp(place, "28")==0)
		return Wnum28;
	else if(strcmp(place, "우정관 1층")==0 || strcmp(place, "우정관") == 0 || strcmp(place, "베르크만스 우정원")==0 || strcmp(place, "BW관")==0 || strcmp(place, "bw관")==0 || strcmp(place, "Bw관")==0 || strcmp(place, "우정원") == 0 ||strcmp(place,"Berchmans Woojung Hall 1st Floor")==0 || strcmp(place, "Berchmans Woojung Hall")==0 ||strcmp(place, "29")==0) 
		return Wnum29;	
	else if(strcmp(place, "체육관 앞문")==0 || strcmp(place, "체육관")==0 ||strcmp(place,"Gymnasium")==0 ||strcmp(place, "30")==0)
		return Wnum30;	
	else if(strcmp(place, "동문회관 4층")==0 || strcmp(place, "동문회관")==0 || strcmp(place, "아루페관")==0 || strcmp(place, "AR관")==0 || strcmp(place, "ar관")==0 || strcmp(place, "Ar관")==0 ||strcmp(place,"Arrupe Hall 4th Floor")==0 || strcmp(place, "31")==0)
		return Wnum31;	
	else if(strcmp(place, "체육관 옆문")==0 ||strcmp(place,"Gymnasium Side Door")==0 ||strcmp(place, "32")==0)
		return Wnum32;	
	else if(strcmp(place, "남문")==0 ||strcmp(place,"South Gate")==0 || strcmp(place, "33")==0)
		return Wnum33;
	else if(strcmp(place, "벨라르미노 학사")==0 || strcmp(place, "벨라")==0 || strcmp(place, "벨라르미노")==0 || strcmp(place, "벨라 기숙사")==0 ||strcmp(place,"Bellarmino Dormitory")==0 || strcmp(place, "34")==0)
		return Wnum34;
	else if(strcmp(place, "리찌과학관 1층")==0 || strcmp(place, "알일")==0 || strcmp(place, "알1")==0 ||strcmp(place,"Ricci Hall 1st Floor")==0 || strcmp(place, "35")==0)
		return Wnum35;	
	else if(strcmp(place, "아담샬관 1층")==0 || strcmp(place, "as1층")==0 || strcmp(place, "에이에스1층")==0 ||strcmp(place,"Adam Schall 1st Floor")==0 || strcmp(place, "36")==0)
		return Wnum36;
	else if(strcmp(place, "김대건관 2층")==0 || strcmp(place, "김대건관 옆문")==0 ||strcmp(place,"Kim Daegon Hall 2nd Floor")==0 || strcmp(place, "37")==0)
		return Wnum37;
	else if(strcmp(place, "메리홀")==0 || strcmp(place, "m관")==0 || strcmp(place, "M관")==0 ||strcmp(place,"Mary Hall")==0 || strcmp(place, "38")==0)
		return Wnum38;
	else if(strcmp(place, "다산관 지하1층")==0 || strcmp(place, "DB")==0 ||strcmp(place,"Dasan Hall B1")==0 ||strcmp(place,"Dasan Hall B1")==0 || strcmp(place, "39")==0) 
		return Wnum39;
	else if(strcmp(place, "우정관 2층")==0 || strcmp(place, "아레떼")==0 ||strcmp(place,"Berchmans Woojung Hall 2nd Floor")==0 ||strcmp(place,"Arete")==0 || strcmp(place, "40")==0)
		return Wnum40;
	else if(strcmp(place, "정하상관 1층")==0 || strcmp(place, "j관 1층")==0 || strcmp(place, "J관 1층")==0 || strcmp(place, "제이관 1층")==0 || strcmp(place, "Jeong Hasang Hall 1st Floor")==0 || strcmp(place, "41")==0)
		return Wnum41;
	else
		return 0;
}

void InitGraph()
{
	int i,j;
	map->vertex_num = 0;
	for(i=1;i<MAX_VERTEX;i++)
	{
		for(j=1;j<MAX_VERTEX;j++)
		{
			map->adj_mat[i][j] = 190000;		//앞서 말했다시피 adj_mat을 weight 기능으로씀
		}
	}
}

void InsertVertex()	
{
	if(map->vertex_num > MAX_VERTEX)
		return ;

	map->vertex_num++;
}

void InsertEdge(int start,int end,int times)
{
	map->adj_mat[start][end] = times;		//이게 가중치 테이블
	map->adj_mat[end][start] = times;
}

void InitSogangMap()
{
	int i;
	/* 모서리 생성*/
	for(i=0;i<MAX_VERTEX;i++)
		InsertVertex();
	for(i=0;i<MAX_EDGE;i++)
	{
		InsertEdge(Warr[i].start_pt,Warr[i].stop_pt,Warr[i].time);  //모서리 생성 
	}
}

void FindShortesttime(int cost_table[MAX_VERTEX][MAX_VERTEX]) //floid-warshall 
{
	int i,j,k;
	/* initailize */
	for(i=1;i<MAX_VERTEX;i++)
	{
		for(j=1;j<MAX_VERTEX;j++)
		{
			cost_table[i][j] = map->adj_mat[i][j];
		}
	}
	for(i=1;i<MAX_VERTEX;i++)
	{
		cost_table[i][i]=0;
	}
	for (i = 1; i < MAX_VERTEX; i++){
		for (j = i+1; j < MAX_VERTEX; j++){
			cost_table[j][i] = cost_table[i][j];
		}
	}
	for(i=1;i<MAX_VERTEX;i++)
	{
		for(j=1;j<MAX_VERTEX;j++)
		{
			for (k = 1; k < MAX_VERTEX; k++)
			{
					if (cost_table[j][k] > cost_table[j][i] + cost_table[i][k])
					{
					cost_table[j][k] = cost_table[j][i] + cost_table[i][k];
					cost_table[k][j] = cost_table[j][k];
					via_table[j][k] = i;
					}
			}
		}
	}
}

void findFast(int start, int end)	//1->2, 2->4 처럼 작은 수에서 큰 수로 갈 때는 역순으로 뽑아주고, 큰 수에서 작은 수로 갈 때는 그대로 뽑음
{
	int routeshow[69] = { 0 };
	int i=0,temp;
	if (start > end){
		temp = start;
		start = end;
		end = temp;
		while (via_table[start][end] != 0){
			i++;
			routeshow[i] = via_table[start][end];
			printf("-> %s ", printPlace(routeshow[i]));
			end = via_table[start][end];
		}
	}
	else{
		while (via_table[start][end] != 0){
			i++;
			routeshow[i] = via_table[start][end];
			end=via_table[start][end];
		}
		while (i >= 1){
			printf("-> %s ", printPlace(routeshow[i]));
			i--;
		}
	}
}
void FindWay(int start, int stop)
{
	int cost[MAX_VERTEX][MAX_VERTEX];
	int i;
	int min=0, sec=0;

	wayinit();
	map= (Graph*)malloc(sizeof(Graph));
	map->adj_mat = (int**)malloc(sizeof(int*)*MAX_VERTEX);
	for (i = 0; i < MAX_VERTEX; i++)
		map->adj_mat[i] = (int*)malloc(sizeof(int)*MAX_VERTEX);

	InitGraph();
	InitSogangMap();
	FindShortesttime(cost);

	system("clear");
	printf("Find route\n");
	printf("--------------------------------------\n");
	min=cost[start][stop]/60;
	sec=cost[start][stop]%60;
	
	if(min==0)
		printf("Minimum time : %d초\n",sec);
	else
		printf("Minimum time : %d분 %d초\n", min, sec);
	printf("Route : \n");
	printf("%s ",printPlace(start));

	findFast(start,stop);
	printf("-> %s", printPlace(stop));
	printf("\n");

	for (i = 0; i < MAX_VERTEX; i++)
		free(map->adj_mat[i]);
	free(map->adj_mat);
}
