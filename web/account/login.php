<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
$authByKey = false;
if(strlen($_GET['key']) == 32)
	$authByKey = true;
if($core->isAuth() && !$authByKey){echo '<script type="text/javascript">location.href="/?p=account"</script>';die('Forbidden');}
$key = $_GET['key'];

function removeKey(){
	$query = $mysqli->query("UPDATE `users` SET AuthKey = '' WHERE AuthKey = '$key'");
}
if($authByKey){
	$query = $mysqli->query("SELECT * FROM `users` WHERE AuthKey = '$key'");
	if(!$query){removeKey();die;}
	$res = mysqli_fetch_array($query);
	if(empty($res['id'])){removeKey();die;}
	$core->auth($res['id'], $res['email'], $res['password']);
	header('Refresh: 0; url=/?p=account');
}
if(isset($_POST['query'])){//Если запрос послан
	//Обрабатываем данные
	$email = $mysqli->real_escape_string(htmlspecialchars(trim($_POST['email'])));
	$password = trim($_POST['password']);
	
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
<script src="js/form-animation.js" type="text/javascript"></script>
<div class="accountWrap">
	<a href="/" class="accountLogo">
	</a>
	<div class="shadowWrap">
		<div class="accountMenu">
			<div class="right">
				<a href="/?p=signup">Зарегистрироваться</a>
			</div>
		</div>
		<div class="accountUploads">
			<form action="#" method="POST" name="regForm" class="regForm">
				<input type="hidden" name="query">
				<div class="formCell">E-Mail: <input type="text" name="email"></div>
				<div class="formCell">Пароль: <input type="password" name="password"></div>
				<center><input type="submit" name="submit" value="Вход"></center>
			</form>
		</div>
	</div>
</div>
<?php
}
$mysqli->close();
?>