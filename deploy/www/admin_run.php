<?php
require_once "php/include.php";
needLogin();
if (!$admin) {
    header('Location: ./');
    exit();
}

switch($_GET["action"])
{
    case 'update_leaderboards':
        chdir("php");
        require_once("update_leaderboards.php");
        break;
    case 'test_bots':
        chdir("php");
        require_once("test_bots.php");
        break;
}