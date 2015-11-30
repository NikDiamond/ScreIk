function getCookie(name) {
  var matches = document.cookie.match(new RegExp(
    "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
  ));
  return matches ? decodeURIComponent(matches[1]) : undefined;
}
function setCookie(name, value, options) {
  options = options || {};

  var expires = options.expires;

  if (typeof expires == "number" && expires) {
    var d = new Date();
    d.setTime(d.getTime() + expires * 1000);
    expires = options.expires = d;
  }
  if (expires && expires.toUTCString) {
    options.expires = expires.toUTCString();
  }

  value = encodeURIComponent(value);

  var updatedCookie = name + "=" + value;

  for (var propName in options) {
    updatedCookie += "; " + propName;
    var propValue = options[propName];
    if (propValue !== true) {
      updatedCookie += "=" + propValue;
    }
  }

  document.cookie = updatedCookie;
}
function moreItems(from){
	$.ajax({
		url: "/acc/getItems.php",
		type: "POST",
		data: {
			"from": from,
			"sort": getCookie('sort')
		},
		success: function(html) {
			$('#moreLink').remove();
			$('.accountUploads').append(html);
			$("html, body").animate({ scrollTop: $(document).height() }, 400);
		}
	});
}
function changeSort() {
	var sortNow = getCookie('sort');
	var newSort = "old";
	if(sortNow == "old") newSort = "new";
	setCookie('sort', newSort, {expires: 30758400, path: '/'});
	location.reload();
}
function calendar() {
	document.getElementById('calendar').focus();
	$(this).siblings('[type="date"]').removeClass('hidden').focus().click();
}
function removeScreen(id){
	$('#itemRemove-'+id).animateRotate(360, 400);
	$.ajax({
		url: "/acc/removeScreen.php",
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