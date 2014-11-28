$.fn.animateRotate = function(angle, duration, easing, complete) {
  var args = $.speed(duration, easing, complete);
  var step = args.step;
  return this.each(function(i, e) {
    args.complete = $.proxy(args.complete, e);
    args.step = function(now) {
      $.style(e, 'transform', 'rotate(' + now + 'deg)');
      if (step) return step.apply(e, arguments);
    };

    $({deg: 0}).animate({deg: angle}, args);
  });
};
$(document).ready(function() {
	$('.accountWrap').css({opacity: 0}).animate({
		opacity: 1
	}, 1000);
	$('body').delegate(".itemRemoved", "mouseenter", function() {
		$(this).animate({
			opacity: 1,
		}, 200);
	}).delegate(".itemRemoved", "mouseleave", function() {
		$(this).animate({
			opacity: 0.8,
		}, 200);
	}).delegate(".itemWrap", "mouseenter", function() {
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
function openDetails() {
	if($('.accountDetails').css('height') == '0px'){//open
		$('.accountDetails').animate({
			opacity: 1,
			height: '60px',
		},200,function() {
			$(this).animate({
				width: '100%',
			}, 200, function() {
				$('.accountDetails span').animate({
					opacity: 1,
				}, 100);
			});
		});
	}else{//close
		$('.accountDetails span').animate({
			opacity: 0,
		}, 100);
		$('.accountDetails').animate({
			width: 0,
		},200,function() {
			$(this).animate({
				opacity: 0,
				height: 0,
			}, 200);
		});
	}
}