<html>
<body>
	<head>
		<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	</head>
<?
	echo("<h1>게시판 쓰기</h1>");
	
	/* form,textarea,input태그를 이용하여 글을 입력하여 등록하거나 다시쓰게 해줌 */
	echo("<form name='input' action='$PHP_SELF' method='post'>
		  <textarea rows=10 cols=30 name='string'> </textarea><BR>
		  <input type='submit' value='등록하기'>
		  <input type='reset' value='다시쓰기'>
		  </form>
		");
	
	/* 입력받은 글을 Data.txt 에 저장한다. */
	$string= $_POST['string'];
	$filename="Data.txt";

	/*글을 썼으면 Data.txt에 저장후 초기화면으로 이동 */
	if($string)
	{
		$fp= fopen($filename,"w");
		fwrite($fp,"$string",strlen($string));
		fclose($fp);
	/* 0.4초안에 저장된 URL로 돌아간다 */
		echo("<meta http-equiv='refresh'
			content='0.4; URL=Board_main.php'>");
	}
	/*되돌아가기 */
	echo("<A HREF= 'Board_main.php'>되돌아가기</a>");
	
?>
</body>
</html>
