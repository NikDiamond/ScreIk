<?php
	session_start();
	include('../connect.php');
	include('../core.class.php');
	$core = new core();
	$core->startAuth();
	$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."' AND `id` <= ".$_POST['from']." ORDER by `id` DESC");
	if($_POST['from'] == 0)
		$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."' ORDER by `id` DESC");
	if($query){
		$num = mysqli_num_rows($query);
		$showMore = false;
		if($num > 30){$num = 30;$showMore = true;}
		$lastItem = 0;
		for($i=1;$i<=$num;$i++){
			$res = mysqli_fetch_array($query);
			$path = 'l/m/'.$res['name'];
			echo '<div class="itemWrap"><a target="_blank" href="l/'.$res['name'].'" class="accountUploadItem" id="'.$res['id'].'">';
			if(file_exists('../'.$path)){
				echo '<img src="'.$path.'">';
			}else{
				echo '<img src="/css/img/oldVersion.png">';
			}
			echo '</a></div>';
			$lastItem = $res['id'];
		}
		if($num == 0 || !$showMore){
			echo '<center style="color: #999">Загружены все скриншоты</center>';
		}
	}
	if($showMore){
?>
		<br>
		<a id="moreLink" onmousedown="$(this).addClass('moreDown').addClass('moreLoad');" onmouseup="$(this).removeClass('moreDown');" href="javascript://" oncontextmenu="moreItems(<?=$lastItem-1?>);return false;" onClick="moreItems(<?=$lastItem-1?>);">Ещё</a>
	<?}?>