$(document).ready(function() {
	$('#mainPage').css({
		marginLeft: '-100%',
	});
	$('.buttons').css({
		opacity: 0,
		width: '350px',
	});
	$('#mainPage').animate({
		marginLeft: 0,
	},800, "easeInCubic", function() {
		$('.buttons').animate({
			width: '392px',
			opacity: 1,
		},500);
	});
	$('.btn').hover(function() {
		$(this).animate({
			opacity: 1,
		},200);
	},function() {
		$(this).animate({
			opacity: 0.8,
		},100);
	});
});