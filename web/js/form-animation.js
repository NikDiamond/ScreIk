$(document).ready(function() {
	$('input[type=text],input[type=password]').hover(function() {
		$(this).animate({
			opacity: 1,
		},200);
	},function() {
		$(this).animate({
			opacity: 0.8,
		},200);
	});
	$('input[type=submit]').hover(function() {
		$(this).animate({
			opacity: 1,
		},200);
	}, function() {
		$(this).animate({
			opacity: 0.6,
		},200);
	});
});