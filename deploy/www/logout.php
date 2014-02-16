<?php
require 'php/functions.php';
sec_session_start();
$sName = session_name();
$_SESSION = array();
$params = session_get_cookie_params();
setcookie($sName, '', 1, $params["path"], $params["domain"], $params["secure"], $params["httponly"]);
setcookie($sName, false);
unset($_COOKIE[$sName]);

session_destroy();
header('Location: /');