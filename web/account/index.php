<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if(!$core->isAuth()){echo '<script type="text/javascript">location.href="/?p=login"</script>';die('Forbidden');}
?>
<script src="js/itemsLoad.js" type="text/javascript"></script>
<script src="js/account-animation.js" type="text/javascript"></script>
<div class="accountWrap">
	<a href="/" class="accountLogo">
	</a>
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
		$lastItem = 0;
		if($query){
			$num = mysqli_num_rows($query);
			for($i=1;$i<=30;$i++){
				$res = mysqli_fetch_array($query);
				$path = 'l/m/'.$res['name'];
				echo '<a href="l/'.$res['name'].'" class="accountUploadItem" id="'.$res['id'].'">';
				if(file_exists($path)){
					echo '<img src="'.$path.'">';
				}else{
					echo '<img src="/css/img/oldVersion.png">';
				}
				echo '</a>';
				$lastItem = $res['id'];
			}
			if($num == 0){
				echo '<center>Нет скриншотов</center>';
			}
		}
		?>
		<br>
		<a id="moreLink" href="javascript://" onClick="moreItems(<?=$lastItem-1?>);">More</a>
		</div>
	</div>
</div>