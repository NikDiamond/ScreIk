<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if($core->isAuth()){echo '<script type="text/javascript">location.href="/"</script>';die('Forbidden');}

if(isset($_POST['query'])){//Если запрос послан
	//Обрабатываем данные
	$email = $mysqli->real_escape_string(htmlspecialchars(trim($_POST['email'])));
	$password = trim($_POST['password']);
	$ip = $_SERVER['REMOTE_ADDR'];
	
	if(!empty($email) && !empty($password) && strlen($email) >= 4 && strlen($email) <= 254 && strlen($password) >= 6){
		if(filter_var($email, FILTER_VALIDATE_EMAIL) && preg_match('/@.+\./', $email)){
			$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email'");
			if($query){
				$res = mysqli_fetch_array($query);
				if(empty($res['id'])){
					$password = md5(md5($password));
					$query = $mysqli->query("INSERT INTO `users` (`email`,`password`,`ip`) VALUES('$email', '$password', '$ip')");
					if($query){
						$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'");
						if($query){
							$res = mysqli_fetch_array($query);
							if(!empty($res['id'])){
								//Записываем cookie, перенаправляем на аккаунт
								$core->auth($res['id'], $res['email'], $res['password']);
								header('Refresh: 0; url=/?p=account');
							}
						}
					}else{
						$core->showError('Ошибка sql.');
					}
				}else{
					$core->showError('Пользователь с таким E-mail уже существует.');
				}
			}else{
				$core->showError('Ошибка sql.');
			}
		}else{
			$core->showError('E-mail введён некорректно.');
		}
	}else{
		$core->showError('Некорректно заполнены данные.');
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