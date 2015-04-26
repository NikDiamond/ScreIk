<?php
include("connect.php");
if($_SERVER['HTTP_USER_AGENT'] != "ScreIk") die();
$act = $_GET['a'];
switch($act){
	case "login":
		$email = $mysqli->real_escape_string(trim(htmlspecialchars($_POST['email'])));
		$password = $mysqli->real_escape_string(trim($_POST['password']));

		if(!empty($email) && !empty($password)){
			$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'") or die('error | Ошибка SQL!');
			$res = mysqli_fetch_array($query);
			if(empty($res['id']))
				die('error | Пользователя с такой связкой "E-mail - пароль" не существует');
			else
				exit('success');
		}else die('error | Поля заполнены некорректно');
	break;
	case "update":
		exit($version.'|'.$link);
	break;
	case "getAuthKey":
		$email = $_POST['email'];
		$password = $_POST['password'];
		$n = mt_rand(0,1000000000000000);
		$key = md5($n);
		$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'");
		$res = mysqli_fetch_array($query);
		if(empty($res['id']) || !$query) die('error | Ошибка авторизации');
		
		$query = $mysqli->query("UPDATE `users` SET AuthKey = '$key' WHERE email = '$email' AND password = '$password'");
		exit($key);
	break;
	case "getStory":
		$email = $_POST['email'];
		$password = $_POST['password'];
		$length = $_POST['length'];
		
		$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'");
		$res = mysqli_fetch_array($query);
		if(empty($res)) die('error | hidden');
		$id = $res['id'];
		$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '$id' ORDER BY `id` DESC LIMIT 3");
		if(!$query) die('error | hidden');
		$num = mysqli_num_rows($query);
		for($i=1;$i<=$num;$i++){
			$res = mysqli_fetch_array($query);
			echo $res['name'].'/'.$res['date'];
			if($i != $num) echo '|';
		}
	break;
	default:
		die('error | hidden');
	break;
}
?>