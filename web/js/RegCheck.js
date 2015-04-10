var xhr;
$(document).ready(function() {
	$("<div id='email-err' class='reg-err'></div>").insertAfter("input[name='email']");
	$("<div id='pass-err' class='reg-err'></div>").insertAfter("input[name='password']");
	$("<div id='pass2-err' class='reg-err'></div>").insertAfter("input[name='rePassword']");
	
	$("input[name='email']").keyup(function() {
		if($("input[name='email']").val() == ""){
			$('#email-err').html('');
			$("input[name='email']").css({backgroundImage: 'none'});
			return;
		}
		if(!ExpCheckEmail($("input[name='email']").val())){
			$('#email-err').html('Некорректный E-mail');
			$("input[name='email']").css({backgroundImage: 'url("/css/img/error.png")'});
		}else{//exp is success
			CheckEmail($("input[name='email']").val());
		}
	});
	//passwords
	$("input[name='password']").keyup(function() {
		if($("input[name='password']").val() == ""){
			$('#pass-err').html('');
			$("input[name='password']").css({backgroundImage: 'none'});
			return;
		}
		if(!ExpCheckPass($("input[name='password']").val())){
			$('#pass-err').html('Короткий пароль');
			$("input[name='password']").css({backgroundImage: 'url("/css/img/error.png")'});
		}else{
			$('#pass-err').html('');
			$("input[name='password']").css({backgroundImage: 'url("/css/img/success.png")'});
		}
	});
	//compare
	$("input[name='rePassword']").keyup(function() {
		if($("input[name='rePassword']").val() == ""){
			$('#pass2-err').html('');
			$("input[name='rePassword']").css({backgroundImage: 'none'});
			return;
		}
		if($("input[name='password']").val() != $("input[name='rePassword']").val()){
			$('#pass2-err').html('Пароли не совпадают');
			$("input[name='rePassword']").css({backgroundImage: 'url("/css/img/error.png")'});
		}else{
			$('#pass2-err').html('');
			$("input[name='rePassword']").css({backgroundImage: 'url("/css/img/success.png")'});
		}
	});
});
function ExpCheckPass(pass){
	if(pass.length >= 6) return true;
}
function ExpCheckEmail(email){
	var re = /^([\w-]+(?:\.[\w-]+)*)@((?:[\w-]+\.)*\w[\w-]{0,66})\.([a-z]{2,6}(?:\.[a-z]{2})?)$/i;
    return re.test(email);
}
function CheckEmail(email){
	if(xhr && xhr.readystate != 4){
		xhr.abort();
	}
	xhr = $.ajax({
		url: '/acc/checking.php',
		method: "POST",
		data: {
			type: "email",
			data: email
		},
		success: function(answer) {
			if(answer != ""){
				$('#email-err').html('Такой E-mail уже зарегистрирован');
				$("input[name='email']").css({backgroundImage: 'url("/css/img/error.png")'});
			}else{
				$('#email-err').html('');
				$("input[name='email']").css({backgroundImage: 'url("/css/img/success.png")'});
			}
		}
	});
}