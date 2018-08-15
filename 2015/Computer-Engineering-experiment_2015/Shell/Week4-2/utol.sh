echo 'Working directory: ' 
read dir													#directory 이름을 입력

if [ -n "$dir" ]											#입력을 받은 $dir이 NULL이 아니면
then
	if [ ! -d "$dir" ]										#$dir이 디렉토리가 아니면
	then
		echo "Error - Wrong directory"						#예외처리
		exit 0												#이후 프로그램을 종료
	fi

	if [ ! -x "$dir" ]										#dir의 탐색 권한이 없다면
	then
		echo "Error - Permission denied"					#예외처리
		exit 0												#이후 프로그램을 종료
	fi
	cd "$dir"												#$dir의 디렉토리로 이동
fi
for str in *												#for 문을 현재 디렉토리에 있는 파일 갯수만큼 돈다.
do
	newname=`echo $str | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`	#이름을 바꿀 변수 newname에 a-z -> A-Z로, A-Z -> a-z로 바꿀 명령어를 넣는다.
	mv $str $newname										#이름을 바꿀 명령어 mv를 이용하여 바꾼다.
done

