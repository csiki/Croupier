<?php
$loggedin = false;
$admin = false;
$tr = NULL;
require "connect_db.php";
require "functions.php";
sec_session_start();
initTranslate($mysqli);
login_check($loggedin, $admin);
//load counter
SQL("INSERT INTO stat_pageload (date, count)
            VALUES (CURRENT_DATE(), 1)
            ON DUPLICATE KEY UPDATE date = VALUES(date), count = count +1;");