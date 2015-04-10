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
					if($password == $_POST['rePassword']){
						$password = md5(md5($password));
						$query = $mysqli->query("INSERT INTO `users` (`email`,`password`,`ip`) VALUES('$email', '$password', '$ip')");
						if($query){
							$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'");
							if($query){
								$res = mysqli_fetch_array($query);
								if(!empty($res['id'])){
									//Записываем cookie, перенаправляем на аккаунт
									$core->auth($res['id'], $res['email'], $res['password']);
									header('Refresh: 0; url=/account');
								}
							}
						}else{
							$core->showError('Ошибка sql');
						}
					}else{
						$core->showError('Пароли не совпадают');
					}
				}else{
					$core->showError('Такой E-mail уже зарегистрирован');
				}
			}else{
				$core->showError('Ошибка sql');
			}
		}else{
			$core->showError('E-mail введён некорректно');
		}
	}else{
		$core->showError('Некорректно заполнены данные');
	}
}else{
?>
<script src="/js/RegCheck.js" type="text/javascript"></script>
<div class="accountWrap">
	<a href="/" class="accountLogo">
	</a>
	<div class="shadowWrap">
		<div class="accountMenu">
			<div class="right">
				<a href="/login">Войти в свой аккаунт</a>
			</div>
		</div>
		<div class="accountUploads">
			<form action="#" method="POST" name="regForm" class="regForm">
				<input type="hidden" name="query">
				<input type="text" name="email" placeholder="E-Mail">
				<input type="password" name="password" placeholder="Пароль">
				<input type="password" name="rePassword" placeholder="Ещё раз пароль">
				<input type="submit" name="submit" value="Регистрация">
			</form>
		</div>
	</div>
</div>
<?php
}
$mysqli->close();
?>