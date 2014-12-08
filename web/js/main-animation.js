$(document).ready(function() {
	$('#mainPage').css({
		marginLeft: '-100%',
	});
	$('.buttons').css({
		opacity: 0,
		width: '352px',
	});
	$('#mainPage').animate({
		marginLeft: 0,
	},800, "easeInCubic", function() {
		$('.buttons').animate({
			width: '452px',
			opacity: 1,
		},500);
	});
	$('.btn').hover(function() {
		$(this).animate({
			opacity: 1,
			backgroundColor: '#fff',
			color: '#777',
		},100);
	},function() {
		$(this).animate({
			opacity: 0.8,
			backgroundColor: 'transparent',
			color: '#fff',
		},100);
	});
});