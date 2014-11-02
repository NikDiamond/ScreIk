<?php
include('connect.php');
$email = $mysqli->real_escape_string(trim(htmlspecialchars($_POST['email'])));
$password = $mysqli->real_escape_string(trim($_POST['password']));

if(!empty($email) && !empty($password)){
	$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'") or die('sqlError');
	$res = mysqli_fetch_array($query);
	if(empty($res['id']))
		die('badLogin');
	else
		die('complete');
}else die("Bad Login");
?>