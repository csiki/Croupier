<?php
$loggedin = false;
$admin = false;
$tr = NULL;
include "php/connect_db.php";
include "php/functions.php";
sec_session_start();
initTranslate($mysqli);
login_check($loggedin, $admin);
//load counter
SQL("INSERT INTO stat_pageload (date, count)
            VALUES (CURRENT_DATE(), 1)
            ON DUPLICATE KEY UPDATE date = VALUES(date), count = count +1;");