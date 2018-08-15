<html>
	<head>
		<meta content= "text/html" charset="utf-8"  http-equiv="Content type">
	</head>
	
	<body>
<?
echo("<font size=20>방문자수 </font> ");
	/*카운트를 기록할 파일 이름을 설정*/
	$filename="counter.txt";

	/* 이전의 카운트를 counter.txt 에서 읽어온다 */
	$fp = fopen($filename,"r");
	$count = fread($fp,filesize($filename));
	fclose($fp);
	
	/*카운트를 1증가 시킨다 단 쿠키를 사용하여 새로고침시에는 방문자수가 올라가지 않게 한다 . */
	if(!$_COOKIE['ip'])
	{
		setcookie("ip",$_SERVER['REMOTE_ADDR']);
		$count=$count+1;
	}
	/* 카운트를 문자로 변환한 후에 이미지로 출력한다 */
	$string=(string)$count;

	for($i=0;$i<strlen($string);$i++)
	{

		$ch = substr($string,$i,1);
		
		switch($ch)
		{
			case 0: echo("<IMG SRC='0.JPG'>"); 
					break;
			case 1: echo("<IMG SRC='1.JPG'>");
					break;	
			case 2: echo("<IMG SRC='2.JPG'>"); 
					break;
			case 3: echo("<IMG SRC='3.JPG'>"); 
					break;
			case 4: echo("<IMG SRC='4.JPG'>"); 
					break;
			case 5: echo("<IMG SRC='5.JPG'>"); 
					break;
			case 6: echo("<IMG SRC='6.JPG'>"); 
					break;
			case 7: echo("<IMG SRC='7.JPG'>"); 
					break;
			case 8: echo("<IMG SRC='8.JPG'>"); 
					break;
			case 9: echo("<IMG SRC='9.JPG'>"); 
					break;
		}
	}
	/*새로운 카운트를 counter.txt 파일에 저장한다 */
	$fp= fopen($filename,"w");
	fwrite($fp,"$count",strlen($count));
	fclose($fp);
?>

	</body>
</html>


