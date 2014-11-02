<?php
$base = 'u419717796_hscr';  			//База данных
$charset = 'utf8'; 						//Кодировка вывода из базы
$host = 'mysql.hostinger.ru'; 			//Хост
$user = 'u419717796_hscr';				//Имя пользователя
$pass = 'UsRuBa220';					//Пароль

$mysqli = new mysqli($host, $user, $pass, $base);
if($mysqli->connect_error)
	die('mysqliError');
$mysqli->query("SET NAMES '$charset'");	//Настройка кодировки
?>