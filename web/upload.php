<?php
include("connect.php");
//authorize check
$email = $_GET['email'];
$passHash = $_GET['passHash'];
$query = $mysqli->query("SELECT * FROM `users` WHERE `email`='$email' AND `password`='$passHash'") or die("error | sqlError");
$res = mysqli_fetch_array($query);
if(empty($res['id']))
	die('error | BadLogin');
	
$uploadDir = 'l/';
$minDir = $uploadDir.'m/';
$isPng = false;//false if jpeg

$ext = strtolower(pathinfo($_FILES['file']['name'], PATHINFO_EXTENSION));
switch($ext){
	case "png"://png file
		$isPng = true;
	break;
	case "jpg":case "jpeg"://jpeg file
		$isPng = false;
		$ext = "jpg";
	break;
	default:
		die("error | wrongFile");
	break;
}
//get new file random name
$nameHash = md5(basename($_FILES['file']['name']).time());
$nameHash = substr($nameHash, 0, 6).substr($nameHash, strlen($nameHash)-6, 6);
$uploadFile = $uploadDir.$nameHash.'.'.$ext;

if(move_uploaded_file($_FILES['file']['tmp_name'], $uploadFile)){//moving file to path
	$name = $nameHash.'.'.$ext;//just file name
	//creating small copy of image
	list($width, $height) = getimagesize($uploadFile);
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
	if($isPng)
		$image = imagecreatefrompng($uploadFile);
	else 
		$image = imagecreatefromjpeg($uploadFile);
	
	imagecopyresampled($image_p, $image, 0, 0, $xMargin, $yMargin, 100, 100, $side, $side);
	imagejpeg($image_p, $minDir.$nameHash.'.jpg');
	
	$mysqli->query("INSERT INTO `screenshots` (`name`, `date`, `userId`) VALUES('$name', NOW(), '".$res['id']."')");
	$uploadFile = explode("/", $uploadFile);
	echo $uploadFile[1];
}else{
	echo 'error';
}
?>