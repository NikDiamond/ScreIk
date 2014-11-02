<?php
include('connect.php');
if(isset($_POST['query'])){//Если запрос послан
	//Обрабатываем данные
	$email = $mysqli->real_escape_string(htmlspecialchars(trim($_POST['email'])));
	$password = trim($_POST['password']);
	$ip = $_SERVER['REMOTE_ADDR'];
	
	if(empty($email) || empty($password) || strlen($email) < 4 || strlen($email) > 254 || strlen($password) < 6) die('dataError');
	if(!filter_var($email, FILTER_VALIDATE_EMAIL) || !preg_match('/@.+\./', $email)) die('emailInvalid');
	
	$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$email'") or die('sqlError');
	$res = mysqli_fetch_array($query);
	if(!empty($res['id'])) die('userExists');

	$password = md5(md5($password));
	$query = $mysqli->query("INSERT INTO `users` (`email`,`password`,`ip`) VALUES('$email', '$password', '$ip')") or die('sqlError');
	exit('complete');
}else{
?>
<!doctype html>
<html lang="ru">
<head>
<title>HotScreen - Registration</title>
</head>
<body>
<form action="#" method="POST" name="regForm" class="regForm">
E-Mail: <input type="text" name="email"><br>
Password: <input type="password" name="password"><br>
<input type="hidden" name="query">
<input type="submit" name="submit"><br>
</form>
</body>
</html>
<?php
}
$mysqli->close();
?>