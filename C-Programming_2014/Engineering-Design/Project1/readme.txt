1)학번:20141575
   Student Id : 20141575

2)이름:임형석
   Name : Hyungsuk Lim

3)프로젝트 진행 환경: 개발프로그램 :putty(서버:cspro.sogang.ac.kr), visual studio 2010
개발공간: r912 ,k512, 집 노트북
   Project environment : Putty(server : cspro.sogang.ac.kr), visual studio 2010
   Development Place : Sogang University R912 lab , K512 lab , Notebook  
4)컴파일 방법:
 putty에서 vi project.c 로 작성한후 math library 를 이용하기 위해 gcc 뒤에 -lm 을 추가해서 gcc project.c -lm 으로 컴파일을 
하였음. 
    How to Compile : gcc project.c -lm

(나중에 filezilla 로 c파일을 옮긴후에 cs20141575.c 로 이름을 바꾸어줌)

5)사용법 및 기타사항 작성
사용법은 gcc cs20141575.c -lm 으로 컴파일을 한후에 ./a.out 으로 프로그램을 실행한다.
Input the angle : 에 각을 넣고
Input the number of terms(3~5): 에 taylor series로 전개될때의 항의 개수를 넣는다(단3~5를 벗어나는 수를 대입하면 error 라는 문구가 뜬다.)
 그러면 처음나오는 <Sin, Cos, Tan function using math library> 에서나오는 sin cos tan값은 진짜 sin,cos,tan값이고
 <Sin, Cos, Tan function using taylor series>에서는 sin cos tan 값은 taylor series 를 이용한 sin cos tan  값이나온다
그 밑 the error rate of sin,cos,tan 에는 taylor series로 얻은값과 math library 로얻은 sin cos tan 값의 오차율을 나타내준다.

After compile by gcc cs20141575.c -lm, execute program by ./a.out
Put angle and num of terms as input, then first values are real sin,cos,tan value and second values are calculated sin,cos,tan value by taylor series
and last values are error rate of math library and taylor series of sin,cos,tan values.