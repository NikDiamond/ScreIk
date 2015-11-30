<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if(!$core->isAuth()){echo '<script type="text/javascript">location.href="/login"</script>';die('Forbidden');}
?>
<script src="/js/itemsLoad.js" type="text/javascript"></script>
<script src="/js/account-animation.js" type="text/javascript"></script>
<div class="accountWrap">
	<a href="/" class="accountLogo">
	</a>
	<?php
	$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."'");
	if($query){
		$num = mysqli_num_rows($query);
	}
	?>
		<div class="accountMenu">
			<div class="acc-icon" onClick="changeSort()"><img src="/css/img/sort<?=$_COOKIE['sort']?>.png"></div>
			<div class="right">
				<a title="Скриншотов: <?=$num?>"><?=$_SESSION['email']?></a>
				<a href="/logout.php">Выход</a>
			</div>
		</div>
	<div class="shadowWrap">
		<div class="accountUploads">
			<script type="text/javascript">moreItems(0);</script>
		</div>
	</div>
</div>