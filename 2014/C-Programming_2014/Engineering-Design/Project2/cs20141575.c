
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int play_game(char id[10][9],char pw[10][9],int score[10]);
void ranking(char id[10][9],int score[10]);
int str_len(char *);
int str_cmp(char *,char *);
void str_cpy(char *,char *);

int main()
{
        int menu,game;
        char id[10][9]={0};
        char pw[10][9]={0};
        int score[10],i;
        FILE *IDPASS;
        int a;
        IDPASS=fopen("user.txt","r");
        for(i=0;i<10;i++)
        {
                fscanf(IDPASS,"%s",id[i]);
                fscanf(IDPASS,"%s",pw[i]);
                fscanf(IDPASS,"%d",&score[i]);
        }

        printf("<menu>\n");                 //메뉴를 구현한 부분이다. 1이입력되면 play_game 2가 입력되면 랭킹이 나오고 3이입력되면 게임이 종료된다.
        printf("1. Play game\n");
        printf("2. Ranking\n");
        printf("3. Exit\n");
        printf("Select menu : \n");
        scanf("%d",&menu);
        if(menu==1)
                game=play_game(id,pw,score);
        else if(menu==2)
                ranking(id,score);
        else if(menu==3)
                return 0;

        fclose(IDPASS);
        return 0;
}
int play_game(char id[10][9],char pw[10][9],int score[10])
{
        char log_id[9],log_pw[9]; //로그인 할때 입력하는 아이디와 pw
        int i,idcmp,pwcmp,temp; 
        int life=10,life_count;//목숨과 나중에 점수에 이용하기위한 life_count
        FILE *wordtxt; 
        char word[50][15]={0}; //단어들을 받아주기위해 필요한 배열
        char category[50][15]; //카테고리들을 받아주기위해 필요한 배열
        char cpy_word[15]={0};  // 단어들을 복사해 언더바로 출력되게 하기위해 필요한 배열
        int hint_count=1,pass_count=1;//힌트들이 얼마나 남았는지 알게해주는 변수.
        char input;//a~z A~Z 1,2를 입력하기위해 필요한 변수
        int char_hint;
        int same_word=0;
        FILE *IDPASS;
        int flag;
        int pass=0;
        int a;
        int leng;//단어의 길이를 받아주기 위한 변수
        int get_score; //얻은 점수를 받아주기위해 필요한 변수
        printf("<Login>\n");   //로그인을 구현한부분인데 idcmp라는건 입력한 id와 저장된id가 같으면 0이 출력되고 다르면 1이출력된다. 그래서 아이디가같지않으면  1이되서 there is no ID 가 출력되고  idcmp가 0이고 pwcmp가 1이면 비밀번호가 틀려서 pw is not correct 가 출력된다.
        printf("ID : ");
        scanf("%s",log_id);
        printf("PW : ");
        scanf("%s",log_pw);
        for(i=0;i<10;i++)
        {
                idcmp=str_cmp(log_id,id[i]);
                if(idcmp==0)
                        break;
        }
        pwcmp=str_cmp(log_pw,pw[i]);
        temp=i;
        if(idcmp==1)
        { 
                printf("there is no ID<%s>\n",log_id);
                return -1;
        }
        else if(idcmp==0&&pwcmp==1)
        {
                printf("PW is not correct!\n");
                return -1;
        }
        else if(idcmp==0&&pwcmp==0)
        {
                printf("Login complete\n");
                wordtxt=fopen("word.txt","r");  //이부분부터 게임을 구현하였는데 일단 단어들이들어있는 word.txt를 열고
                for(i=0;i<50;i++)
                { 
                        fscanf(wordtxt,"%s",word[i]);    //이 부분들은 단어들을 배열에받고 category 도 배열에받는 부분이다.
                        fscanf(wordtxt,"%s",category[i]);
                }
                while(life>0)
                {
                        life_count=life;
                        srand(time(NULL));
                        a =rand()%50;
                        leng=str_len(word[a]);   //단어의길이를 구하여 그길이 만큼 언더바를 적어 놓은 부분이다.
                        str_cpy(word[a],cpy_word);    
                        for(i=0;i<leng;i++)
                                cpy_word[i]='_';
                        cpy_word[leng]='\0';
                        while(str_cmp(word[a],cpy_word)==1 && life>0) 
                        {

                                printf("\nLife : %d\n",life);
                                printf("Word : ");
                                for(i=0;i<leng;i++)
                                        printf("%c ",cpy_word[i]);
                                printf("\nCategory : %s\n",category[a]);
                                printf("1. Character hint(count : %d) 2. Word pass(count : %d)\n",hint_count,pass_count);
                                printf("Input alphabet or item number: ");
                                scanf(" %c",&input);
                                if(input>='a'&&input<='z')   //적은 부분이 a~z일때 그 알파벳이 맞으면 언더바를 그 알파벳으로 바꾸어 주고 틀리면은 언더바는 언더바로 그냥 놓고 목숨을 깎는다.
                                {
                                        flag=0;
                                        for(i=0;i<leng;i++)
                                        {
                                                if(word[a][i]==input)
                                                {
                                                        cpy_word[i]=input;
                                                        flag=1;
                                                }
                                        }
                                        if(flag==0)
                                                life--;
                                }
                                else if(input>='A'&&input<='Z')//적은 부분이 A~Z일때 그알파벳이 맞으면 언더바를 그알파벳으로 바꾸어준다.
                                {
                                        input=input-'A'+'a';
                                        for(i=0;i<leng;i++)
                                        {
                                                if(word[a][i]==input)
                                                {
                                                        cpy_word[i]=input;
                                                        flag=1;
                                                }

                                        }
                                        if(flag==0)
                                                life--;
                                }
                                else if(input=='1'&&hint_count==1) //적은 부분이 1일때 character hint를 사용한다. 힌트가 1일때 작동
                                {
                                        printf("Select word column to see(1~%d)",leng);
                                        scanf("%d",&char_hint);
                                        printf("Alphabet which you select is %c",word[a][char_hint-1]);
                                        hint_count--;
                                        continue;
                                }
                                else if(input='2'&&pass_count==1) //적은 것이 2일때 wordpass를 사용. wordpass가1일때 사용
                                {
                                        printf("You use word pass!!\n");
                                        pass_count--;
                                        pass=1;
                                        break;
                                }
                                else if(input=='1'&&hint_count==0)  //적은부분이 1이지만 힌트가 0일때 오류가 출력
                                {
                                        printf("Error! You don't have character hint\n");
                                        continue;
                                }
                                else if(input=='2'&&pass_count==0) //적은 것이 2이지만 wordpass가 0이어서 오류출력
                                {
                                        printf("error! You don`t have Word pass\n");
                                        continue;
                                }
                                else printf("Error !,Put Alphabet or item number\n");
                        }        
                        if(life_count==life&&life!=0)   //하나도틀리지않앗을때 점수구현
                                get_score=life+(leng*2);
                        else if(life_count!=life&&life!=0)  //한번이상틀렸을때 점수구현
                                get_score=life+leng;
                        if(pass==1) 
                        {
                                pass=0;
                                continue;
                        }
                        else      // 문제를 맞추었을때 출력되는 부분. 얻은 점수 현재점수 맞춘후 점수
                        {
                                printf("Correct! The word is %s\n",word[a]);
                                printf("The score that you get is %d\n",get_score);
                                printf("Your current score is %d\n",score[temp]);
                                score[temp]+=get_score;
                                printf("Your total score is %d",score[temp]);
                                IDPASS=fopen("user.txt","w");
                                for(i=0;i<10;i++)
                                        fprintf(IDPASS,"%s %s %d\n",id[i],pw[i],score[i]);
                                fclose(IDPASS);
                        }
                
                }      
        }
        return 0;
}

void ranking(char id[10][9],int score[10])  // 랭킹구현
{
        FILE *IDPASS;
        int temp,i,j,index,temp1,t;
        char cpy_id[10][9]={0};
        int aft_id[10]={0};
        int bef_score[10]={0};

        printf("%5s%10s%15s\n","Rank","User ID","Score")  // 유저아이디와 스코어를 입력하는부분
        for(i=0;i<10;i++)
                bef_score[i]=score[i];

        for(i=0;i<9;i++)
        {
                index=i;
                for(j=i+1;j<10;j++)
                {
                        if(score[index]<score[j])
                                index=j;
                }
                temp=score[i];
                score[i]=score[index];
                score[index]=temp;    //점수가제일것이 score[0]에 들어감
        }
        for(t=0;t<10;t++)
                str_cpy(id[t],cpy_id[t]);
        for(i=0;i<10;i++)
        {
                for(j=0;j<10;j++)
                {
                        if(bef_score[i]==score[j])
                                aft_id[j]=i;
                }
        }          //점수가제일큰 아이디가 id[0]에 들어가게함

        for(i=0;i<10;i++)                    
                printf("%5d%10s%15d\n",i+1,cpy_id[aft_id[i]],score[i]);


}
int str_len(char len[])  // 단어의 길이를 return 해주는 함수
{
        int i=0,length=0;
        while(1)
        {
                if(len[i]=='\0')  // len[i]가 '\0'일때까지의 길이를 구함
                        break;

                length++;
                i++;
        }
        return length;
}

int str_cmp(char s1[],char s2[])  // 두 배열을 비교하여 두개가 같으면 0 두개의 길이가다르거나 두개의 배열이다르면 1이 return 된다.
{
        int i,x=0,t;
        if(str_len(s1)!=str_len(s2))
                x=1;
        else if(str_len(s1)==str_len(s2))
        {
                t=str_len(s1);
                for(i=0;i<t;i++)
                {
                        if(s1[i]!=s2[i])
                                x=1;
                }
        }
        return x;
}

void str_cpy(char x[], char y[])  //하나의 배열을 다른 배열에 복사하는 함수
{
        int i=0;
        while(1)
        {
                if(x[i]=='\0')
                        break;
                y[i]=x[i];
                i++;
        }
}
