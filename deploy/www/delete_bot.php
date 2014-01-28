<?php
include "php/include.php";
needLogin();
if (isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $res = SQL("DELETE FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $_GET["botID"]);
    if ($res == null) //not found botID
    {
        echo "0";
        exit();
    }

    SQL("DELETE FROM games_by_bots WHERE botID = ?", $_GET["botID"]);

    $leaderboards = SQL("SELECT tableName FROM leaderboards");
    foreach ($leaderboards as $leaderboard) {
        SQL("DELETE FROM ". $leaderboard["tableName"] ." WHERE botID = ?", $_GET["botID"]);
    }

    $files = glob(_BOT_AI_RELATIVE_PATH_ . $_GET["botID"] . "/" . '*', GLOB_MARK);
    foreach ($files as $file) {
        $s = unlink($file);
    }
    rmdir(_BOT_AI_RELATIVE_PATH_ . $_GET["botID"] . "/");
    echo "1";
} else
    echo "0";