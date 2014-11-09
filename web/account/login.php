<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if($core->isAuth()){echo '<script type="text/javascript">location.href="/"</script>';die('Forbidden');}

if(isset($_POST['query'])){//Если запрос послан
	//Обрабатываем данные
	$email = $mysqli->real_escape_string(htmlspecialchars(trim($_POST['email'])));
	$password = md5(md5(trim($_POST['password'])));
	
	$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'");
	if($query){
		$res = mysqli_fetch_array($query);
		if(!empty($res['id'])){
			$core->auth($res['id'], $res['email'], $res['password']);
			header('Refresh: 0; url=/?p=account');
		}else{
			$core->showError('Пользователя с такой связкой "E-mail - пароль не существует".');
		}
	}else{
		$core->showError('Ошибка запроса');
	}
}else{
?>
<form action="#" method="POST" name="regForm" class="regForm">
E-Mail: <input type="text" name="email"><br>
Password: <input type="password" name="password"><br>
<input type="hidden" name="query">
<input type="submit" name="submit"><br>
</form>
<?php
}
$mysqli->close();
?>