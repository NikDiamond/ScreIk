$(document).ready(function() {
	$('.logo').css({
		opacity: 0
	});
	$('.buttons').css({
		opacity: 0,
		width: '352px'
	});
	$('.logo').animate({
		opacity: 1
	},600);
	$('.buttons').animate({
		width: '452px',
		opacity: 1
	},500);
});