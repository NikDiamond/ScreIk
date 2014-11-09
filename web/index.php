<?
session_start();
$app = true;
include('connect.php');
include('core.class.php');
$core = new core();
$core->startAuth();
?>
<!doctype html>
<html lang="ru-RU">
<head>
	<meta http-equiv="content-language" content="ru-RU">
	<meta http-equiv="Content-type" content="text/html" charset="utf-8">
	<meta name="Robots" content="NOINDEX,FOLLOW">
	<meta name="AUTHOR" content="NikDiamond, Dmitry Bardyshev">
	
	<link rel="stylesheet" href="css/style.css">
	<link rel="shortcut icon" href="css/img/icon.ico" type="image/x-icon">
	<title>ScreIk</title>
</head>
<body>
<div class="wrapper">
	<?php
	$incFile = $_GET['p'];
	include($core->getPagePath($incFile));
	?>
</div>
</body>
</html>