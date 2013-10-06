<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Levys
 * Date: 2013.09.24.
 * Time: 20:11
 * To change this template use File | Settings | File Templates.
 */
define("HOST", "localhost"); // The host you want to connect to.
define("USER", "root"); // The database username.
define("PASSWORD", ""); // The database password.
define("DATABASE", "croupierweb"); // The database name.

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