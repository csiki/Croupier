<?php
define("HOST", "localhost");
define("USER", "root");
define("PASSWORD", "asdasd1337");
define("DATABASE", "croupier");

$DB = null;
$mysqli = new mysqli(HOST, USER, PASSWORD, DATABASE);
if ($mysqli->connect_errno) {
    dieDb($mysqli);
}
$DB = $mysqli;
$mysqli->set_charset("utf8");

function dieDb()
{
    global $DB;
    die("Failed to connect to MySQL: " . $DB->error);
}