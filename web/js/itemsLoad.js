function moreItems(from){
	$.ajax({
		url: "/account/getItems.php",
		type: "POST",
		data: {
			"from": from,
		},
		success: function(html) {
			$('#moreLink').remove();
			$('.accountUploads').append(html);
		}
	});
}
function removeScreen(id){
	$('#itemRemove-'+id).animateRotate(360, 400);
	$.ajax({
		url: "/account/removeScreen.php",
		type: "POST",
		data: {
			"id": id,
		},
		success: function(html) {
			if(html === "true"){
				$('#'+id).animateRotate(360, 700, 'swing', function(){
					$("<div class='itemRemoved'>Удалено</div>").prependTo(this).animate({
						marginLeft: 0,
					}, 500)
				});
				$('#'+id).css({borderColor: 'transparent'});
				$('#'+id+' img').animate({opacity: '0.4'},200,function() {
					$(this).unwrap();
					$('#'+id+' a').remove();
				});
				$('#itemTop-'+id).animate({
					marginTop: '-20px',
				}, 100, function() {
					$(this).remove();
				});
				$('#itemBottom-'+id).animate({
					marginTop: '100px',
				}, 100, function() {
					$(this).remove();
				});
			}
		}
	});
}