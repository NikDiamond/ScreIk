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
	<script>
	  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
	  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
	  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
	  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

	  ga('create', 'UA-51043230-2', 'auto');
	  ga('send', 'pageview');

	</script>
	<meta http-equiv="content-type" content="text/html; charset=utf-8">
	<meta name="description" content="Программа для создания быстрых скриншотов ScreIk">
	<meta name="Keywords" content="ScreIk NikDiamond Screenshoot fast скрин создать скрин быстро скриншот заскринить снимок экрана быстрый скриншот программа удобно легко просто">
	<meta name="revisit-after" content="1 day">
	<meta name="AUTHOR" content="NikDiamond, Dmitry Bardyshev">
	
	<link rel="stylesheet" href="/css/style.css">
	<link rel="shortcut icon" href="/css/img/icon.ico" type="image/x-icon">
	<title>ScreIk <?=(!empty($_GET['p']) ? " - ".ucfirst($_GET['p']) : "")?></title>
	<script src="/js/jquery-1.11.1.min.js" type="text/javascript"></script>
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