$(document).ready(function() {
	$('.accountWrap').css({opacity: 0}).animate({
		opacity: 1
	}, 1000);
	$('body').delegate(".itemWrap", "mouseenter", function() {
		var id = $(this).attr('id');
		$('#itemTop-'+id).animate({
			marginTop: 0,
		}, 100);
		$('#itemBottom-'+id).animate({
			marginTop: '65px',
		}, 100);
	}).delegate(".itemWrap", "mouseleave", function() {
		var id = $(this).attr('id');
		$('#itemTop-'+id).animate({
			marginTop: '-20px',
		}, 100);
		$('#itemBottom-'+id).animate({
			marginTop: '100px',
		}, 100);
	});
});