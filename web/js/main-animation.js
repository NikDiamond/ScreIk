$(window).resize(function() {
	pos();
});
$(document).ready(function() {
	$('.description').css('opacity', 0);
	$('.description').animate({
		opacity: 1
	},400);
	
	pos();
});
function pos(){
	var descw = $('.description').innerWidth();
	var blockw = $('.desc-block').innerWidth();
	var count = Math.floor(descw/(blockw+10));
	
	var blocksCount = $('.desc-block').length;
	if(count > blocksCount) count = blocksCount;
	if(count > 2) count = 2;
	
	if(blocksCount == 0) $('.description').css('display','none');
	
	switch(count){
		case 1:
			$('.desc-block').css({
				'float': 'none',
				'margin': '0 auto'
				});
		break;
		case 2: case 3:
			var leftSpace = descw-blockw*count;
			var margin = leftSpace/count-2;
			
			$('.desc-block').css({
				'float': 'left',
				'margin-right': margin/2+'px',
				'margin-left': margin/2+'px'
			});
		break;
	}
}