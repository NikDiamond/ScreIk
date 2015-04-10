<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if($core->isAuth()){kick();}
function kick(){
	echo '<script type="text/javascript">location.href="/account"</script>';die('Forbidden');
}
$key = $_GET['key'];
if(!empty($key)){
	if(isset($_POST['query'])){//Если запрос послан
		$password = $_POST['password'];
		if(strlen($password) >= 6 && $password == $_POST['rePassword']){
			$password = md5(md5($password));
			$query = $mysqli->query("UPDATE `users` SET `password` = '$password', `ResetKey` = '' WHERE `ResetKey` = '$key'");
			if($query){
				$core->showError('Пароль успешно изменён');
			}else $core->showError('Ошибка восстановления');
		}else $core->showError('Некорректные данные');
	}else{
		$query = $mysqli->query("SELECT * FROM `users` WHERE `ResetKey` = '$key'");
		if($query){
			$res = mysqli_fetch_array($query);
			if(!empty($res['id'])){
			?>
<script src="/js/RegCheck.js" type="text/javascript"></script>
<div class="accountWrap">
	<a href="/" class="accountLogo">
	</a>
	<div class="shadowWrap">
		<div class="accountMenu"><div class="left"><a>Придумайте новый пароль</a></div></div>
		<div class="accountUploads">
			<form action="#" method="POST" name="regForm" class="regForm">
				<input type="hidden" name="query">
				<input type="password" name="password" placeholder="Пароль">
				<input type="password" name="rePassword" placeholder="Ещё раз пароль">
				<input type="submit" name="submit" value="Подтвердить">
			</form>
		</div>
	</div>
</div>
			<?php
			}else kick();
		}else kick();
	}
}else{
	if(isset($_POST['query'])){//Если запрос послан
		$email = $_POST['email'];
		$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email'");
		if($query){
			$res = mysqli_fetch_array($query);
			if(!empty($res['id'])){
				$key = md5(time().$email.$_SERVER['REMOTE_ADDR']);
				$query = $mysqli->query("UPDATE `users` SET `ResetKey` = '$key' WHERE `email` = '$email'");
				if($query){
					$link = "http://screik.tk/?p=resetPassword&key=".$key;
					mail($email, 'Reset password on ScreIk', "Для восстановления пароля перейдите по этой ссылке: $link");
					$core->showError('Ссылка для восстановления пароля отправлена на ваш E-Mail');
				}else{
					$core->showError('Ошибка восстановления пароля');
				}		
			}else{
				$core->showError('Пользователь с таким E-Mail не зарегистрирован');
			}			
		}
	}else{
?>
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
				<br><input type="submit" name="submit" value="Восстановить">
			</form>
		</div>
	</div>
</div>
<?php
	}
}
$mysqli->close();
?>