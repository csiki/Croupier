<?php
$loggedin = false;
$admin = false;
$tr = NULL;
include "php/connect_db.php";
include "php/functions.php";
sec_session_start();
initTranslate($mysqli);
login_check($loggedin, $admin);