<?php
session_start();
setcookie("id", "", time() - 3600, "/");
setcookie("email", "", time() - 3600, "/");
setcookie("password", "", time() - 3600, "/");
unset($_SESSION['id']);
unset($_SESSION['email']);
unset($_SESSION['password']);
session_destroy();
header('Refresh: 0; url=/');
?>