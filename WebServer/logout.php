<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Levys
 * Date: 2013.09.24.
 * Time: 20:00
 * To change this template use File | Settings | File Templates.
 */
include 'functions.php';
sec_session_start();
// Unset all session values
$_SESSION = array();
// get session parameters
$params = session_get_cookie_params();
// Delete the actual cookie.
setcookie(session_name(), '', time() - 42000, $params["path"], $params["domain"], $params["secure"], $params["httponly"]);
// Destroy session
session_destroy();
header('Location: ./');