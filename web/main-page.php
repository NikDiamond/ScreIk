<?
	if(!$app){header("HTTP/1.0 403 Forbidden");exit;}
	$link = "https://docs.google.com/uc?authuser=0&id=0BwEu8gfpVIkFcEtHaWl4ZlNnekU&export=download";
?>
<script src="js/main-animation.js" type="text/javascript"></script>
<div class="mainPage" id="mainPage">
	<div class="logo">ScreIk</div>
	<div class="buttons">
		<a href="<?=$link?>" class="btn downloadBtn">скачать<span>ScreIk <?=$version?></span></a>
		<a href="/account" class="btn accountBtn">аккаунт</a>
	</div>
	<div class="description">
		<div class="desc-block">
			<img src="css/img/presentation/1.png">
			<p align="center">
				Создайте скриншот в один клик!
			</p>
		</div>
		<div class="desc-block">
			<img src="css/img/presentation/2.png">
			<p>
				Ссылка на него будет скопирована в буфер обмена
			</p>
		</div>
		<div class="desc-block">
			<img src="css/img/presentation/3.png">
			<p>
				Теперь вы можете отправить ссылку своим друзьям
			</p>
		</div>
		<div class="desc-block">
			<img src="css/img/presentation/4.png">
			<p>
				Просматривайте историю скриншотов
				и управляйте ими в своём аккаунте
			</p>
		</div>
	</div>
</div>