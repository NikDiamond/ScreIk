<?php
	session_start();
	include('../connect.php');
	include('../core.class.php');
	$core = new core();
	$core->startAuth();
	$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `id` = '".$_POST['id']."' AND `userId` = ".$_SESSION['id']);
	if($query){
		$res = mysqli_fetch_array($query);
		if(!empty($res['id'])){
			$remQ = $mysqli->query("DELETE FROM `screenshots` WHERE `id` = ".$_POST['id']);
			if($remQ){
				if(unlink("../l/".$res['name']) && unlink("../l/m/".substr($res['name'], 0, -4).'.jpg')){
					die("true");
				}
			}else{
				die("error");
			}
		}else{
			die("error");
		}
	}else{
		die('error');
	}
?>