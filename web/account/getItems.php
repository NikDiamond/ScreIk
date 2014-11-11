<?php
	session_start();
	include('../connect.php');
	include('../core.class.php');
	$core = new core();
	$core->startAuth();
	$query = $mysqli->query("SELECT * FROM `screenshots` WHERE `userId` = '".$_SESSION['id']."' AND `id` <= ".$_POST['from']." ORDER by `id` DESC");
	if($query){
		$num = mysqli_num_rows($query);
		$showMore = false;
		if($num > 30){$num = 30;$showMore = true;}
		$lastItem = 0;
		for($i=1;$i<=$num;$i++){
			$res = mysqli_fetch_array($query);
			$path = 'l/m/'.$res['name'];
			echo '<a href="l/'.$res['name'].'" class="accountUploadItem" id="'.$res['id'].'">';
			if(file_exists('../'.$path)){
				echo '<img src="'.$path.'">';
			}else{
				echo '<img src="/css/img/oldVersion.png">';
			}
			echo '</a>';
			$lastItem = $res['id'];
		}
		if($num == 0 || !$showMore){
			echo '<center style="color: #999">Загружены все скриншоты</center>';
		}
	}
	if($showMore){
?>
		<br>
		<a id="moreLink" href="javascript://" onClick="moreItems(<?=$lastItem-1?>);">More</a>
	<?}?>