<?php
include("connect.php");
$email = $_GET['email'];
$passHash = $_GET['passHash'];
$query = $mysqli->query("SELECT * FROM `users` WHERE `email`='$email' AND `password`='$passHash'") or die("sqlError");
$res = mysqli_fetch_array($query);
if(empty($res['id']))
	die('badLogin');

$uploadDir = 'uploads/';
$nameHash = md5(basename($_FILES['file']['name']).time());
$nameHash = substr($nameHash, 0, 6).substr($nameHash, strlen($nameHash)-6, 6);
$uploadFile = $uploadDir.$nameHash.'.png';
if(move_uploaded_file($_FILES['file']['tmp_name'], $uploadFile)){
	$nm = $nameHash.'.png';
	$mysqli->query("INSERT INTO `screenshots` (`name`, `userId`) VALUES('$nm', '".$res['id']."')");
	echo 'http://'.$_SERVER['HTTP_HOST'].'/'.$uploadFile;
}else{
	echo 'error';
}
?>