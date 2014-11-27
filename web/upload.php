<?php
include("connect.php");
$email = $_GET['email'];
$passHash = $_GET['passHash'];
$query = $mysqli->query("SELECT * FROM `users` WHERE `email`='$email' AND `password`='$passHash'") or die("sqlError");
$res = mysqli_fetch_array($query);
if(empty($res['id']))
	die('badLogin');

$uploadDir = 'l/';
$minDir = $uploadDir.'m/';
$nameHash = md5(basename($_FILES['file']['name']).time());
$nameHash = substr($nameHash, 0, 6).substr($nameHash, strlen($nameHash)-6, 6);
$uploadFile = $uploadDir.$nameHash.'.png';
if(move_uploaded_file($_FILES['file']['tmp_name'], $uploadFile)){
	$nm = $nameHash.'.png';
	$forResize = $uploadDir.$nm;
	
	list($width, $height) = getimagesize($forResize);
	if($width < $height){//Выбираем квадрат
		$side = $width;
		$yMargin = ($height - $width)/2;
		$xmargin = 0;
	}else{
		$side = $height;
		$xMargin = ($width - $height)/2;
		$ymargin = 0;
	}
	$image_p = imagecreatetruecolor(100, 100);
	$image = imagecreatefrompng($forResize);
	imagecopyresampled($image_p, $image, 0, 0, $xMargin, $yMargin, 100, 100, $side, $side);
	imagepng($image_p, $minDir.$nm, 0);
	
	$mysqli->query("INSERT INTO `screenshots` (`name`, `date`, `userId`) VALUES('$nm', NOW(), '".$res['id']."')");
	echo 'http://'.$_SERVER['HTTP_HOST'].'/'.$uploadFile;
}else{
	echo 'error';
}
?>