<?php
	require_once('../connect.php');
	$type=$_POST['type'];
	$data=$_POST['data'];
	switch($type){
		case 'email':
			$query = $mysqli->query("SELECT * FROM `users` WHERE `email` = '$data'");if(!$query) die('error');
			$res = mysqli_fetch_array($query);
			if(!empty($res['id'])) exit('exists');
		break;
	}
	$mysqli->close();
?>