<?if(!$app){header("HTTP/1.0 403 Forbidden");exit;}?>
<div class="logowrap">
	<div class="logo">
	</div>
	<div class="links">
		<div class="regLinks">
		<?if(!$core->isAuth()){?>
			<a href="/?p=login">Вход</a>
			<a href="/?p=signup">Регистрация</a>
		<?}else{?>
			<a href="/?p=account">Аккаунт</a>
		<?}?>
		</div>
	</div>
</div>