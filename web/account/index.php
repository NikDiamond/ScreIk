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
		<script type="text/javascript">moreItems(0);</script>
		</div>
	</div>
</div>