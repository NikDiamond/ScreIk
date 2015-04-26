<?php
$base = '';  				//База данных
$charset = 'utf-8'; 		//Кодировка вывода из базы
$host = ''; 				//Хост
$user = '';					//Имя пользователя
$pass = '';					//Пароль

$version = "0.8b";
$link = "http://".$_SERVER['SERVER_NAME']."/update/ScreIk.sc";

$mysqli = new mysqli($host, $user, $pass, $base);
if($mysqli->connect_error)
	die('mysqliError');
$mysqli->query("SET NAMES '$charset'");	//Настройка кодировки
$mysqli->query("SET time_zone = 'Europe/Moscow'");
$mysqli->query("SET time_zone = '+03:00'");
$mysqli->query("SET @@session.time_zone = '+03:00'");
?>