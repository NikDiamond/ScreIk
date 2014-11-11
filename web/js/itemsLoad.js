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