<?php
class core{
	function __construct(){}
	public function showError($message){
		echo $message;
		return true;
	}
	public function getPagePath($name){
		switch($name){
			case 'login':
				return 'account/login.php';
			break;
			case 'signup':
				return 'account/signup.php';
			break;
			case 'account':
				return 'account/index.php';
			break;
			default:
				return 'main-page.php';
			break;
		}
	}
	public function auth($id, $email, $password){
		$_SESSION['id'] = $id;
		$_SESSION['email'] = $email;
		$_SESSION['password'] = md5(md5($password));
		setcookie("id", $id, time()+1209600, "/");
		setcookie("email", $email, time()+1209600, "/");
		setcookie("password", md5(md5($password)), time()+1209600, "/");
		return true;
	}
	public function isAuth(){
		if(!empty($_SESSION['email']) && !empty($_SESSION['password'])){
			return true;
		}else{
			return false;
		}
	}
	public function startAuth(){
		if(empty($_SESSION['email']) || empty($_SESSION['password'])){
			if(!empty($_COOKIE['email']) && !empty($_COOKIE['password'])){
				$_SESSION['id'] = $_COOKIE['id'];
				$_SESSION['email'] = $_COOKIE['email'];
				$_SESSION['password'] = $_COOKIE['password'];
				return true;
			}else return false;
		}else return false;
	}
}
?>