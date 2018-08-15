<html>
<body>
	<head>
		<meta http-equiv="Content-type" content="text/html" charset="utf-8">
	</head>

<?
	echo("<h1>게시판 읽기</h1>");
	/* Data.txt에 저장된 내용을 읽고 string 에 저장한다 */
	$file = "Data.txt";	
	$fp = fopen("$file","r");
	$string = fread($fp,filesize($file)); 
	fclose($fp);
	/* textarea로 string에 저장된 내용을 출력 */
	echo("<textarea rows=10 cols=30 name='output'>$string </textarea> <BR><BR>");
	echo("<A HREF = 'Board_main.php'>되돌아가기</a>");
?>
</body>
</html>
