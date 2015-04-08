<?php
if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
if(!$core->isAuth()){echo '<script type="text/javascript">location.href="/login"</script>';die('Forbidden');}
?>
<script src="/js/itemsLoad.js" type="text/javascript"></script>
<script src="/js/account-animation.js" type="text/javascript"></script>
<div class="accountWrap">
	<!--<a href="/" class="accountLogo">
	</a>-->
	<script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
	<!-- screik account -->
	<ins class="adsbygoogle"
		 style="display:inline-block;width:728px;height:90px"
		 data-ad-client="ca-pub-8208099690229615"
		 data-ad-slot="9593787680"></ins>
	<script>
	(adsbygoogle = window.adsbygoogle || []).push({});
	</script>
	<div class="shadowWrap">
		<div class="accountMenu">
			<div class="left">
				<a href="/">Главная</a>
			</div>
			<div class="right">
				<a href="javascript://" onClick="openDetails();"><?=$_SESSION['email']?></a>
				<a href="/logout.php">Выход</a>
			</div>
		</div>
		<div class='accountDetails'>
			<?php
			$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."'");
			if($query){
				$num = mysqli_num_rows($query);
				echo '<span>Количество скриншотов: '.$num.'</span>';
			}
			?>
		</div>
		<div class="accountUploads">
			<script type="text/javascript">moreItems(0);</script>
		</div>
	</div>
</div>