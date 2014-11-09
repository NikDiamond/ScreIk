<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if(!$core->isAuth()){echo '<script type="text/javascript">location.href="/"</script>';die('Forbidden');}
?>

<div class="accountWrap">
	<div class="accountLogo">
	</div>
	<div class="shadowWrap">
		<div class="accountMenu">
			<div class="left">
				<a href="/">Главная</a>
			</div>
			<div class="right">
				<a href="javascript://"><?=$_SESSION['email']?></a>
				<a href="logout.php">Выход</a>
			</div>
		</div>
		<div class="accountUploads">
		<?php
		$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."' ORDER by `id` DESC");
		if($query){
			$num = mysqli_num_rows($query);
			for($i=1;$i<=$num;$i++){
				$res = mysqli_fetch_array($query);
				$path = 'l/m/'.$res['name'];
				echo '<a href="l/'.$res['name'].'" class="accountUploadItem">';
				if(file_exists($path)){
					echo '<img src="'.$path.'">';
				}else{
					echo '<img src="/css/img/oldVersion.png">';
				}
				echo '</a>';
			}
		}
		?>
		</div>
	</div>
</div>