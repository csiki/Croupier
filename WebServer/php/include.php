<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Levys
 * Date: 2013.09.25.
 * Time: 20:42
 * To change this template use File | Settings | File Templates.
 */
$loggedin = false;
$tr = NULL;
include "php/connect_db.php";
include "php/functions.php";
sec_session_start();
initTranslate($mysqli);
$loggedin = login_check($mysqli);