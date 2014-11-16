<?php
include("connect.php");
$act = $_GET['a'];
switch($act){
	case "getAuthKey":
		$email = $_POST['email'];
		$password = $_POST['password'];
		$n = mt_rand(0,1000000000000000);
		$key = md5($n);
		$query = $mysqli->query("UPDATE `users` SET AuthKey = '$key' WHERE email = '$email' AND password = '$password'");
		if(!$query)die('auth-error');
		else exit($key);
	break;
}
?>