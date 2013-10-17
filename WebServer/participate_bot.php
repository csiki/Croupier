<?php
include "php/include.php";
needLogin();
if (isset($_GET["leaderBoard"]) && isset($_GET["botID"]) && isset($_GET["action"])) {
    //validate
    $lb = $_GET["leaderBoard"];
    $botID = $_GET["botID"];
    $action = $_GET["action"];
    if (is_numeric($lb) && is_numeric($botID) && ($action == "1" || $action == "0")) {
        //get bot and add to leaderboard
        $res = SQL("SELECT state FROM bots WHERE id = ?", $botID);
        if($res == null || $res[0]["state"] != "ok")
            die("Invalid request");
        if($action == "1")
            SQL("INSERT INTO leaderboard".$_GET["leaderBoard"]." (botId) values(?)", $botID);
        else
            SQL("DELETE FROM leaderboard".$_GET["leaderBoard"]." WHERE botId = ?", $botID);
        echo "1";
    } else
        die("Invalid request");
} else
    die("Invalid request");