<?php
require_once "php/include.php";
require_once "php/leaderboard.php";
needLogin();

if (isset($_GET["leaderboardID"]) && isset($_GET["botID"]) && isset($_GET["action"])) {
    //validate
    $lbID = $_GET["leaderboardID"];
    $botID = $_GET["botID"];
    $action = $_GET["action"];
    if (is_numeric($lbID) && is_numeric($botID) && ($action == "1" || $action == "0")) {
        //get bot and add to leaderboard
        $res = SQL("SELECT COUNT(*) FROM bots WHERE id = ? AND accountID = ?", $botID, $_SESSION["accountID"]);
        if($res == null)
        {
            echo 0;
            exit();
        }
        $leaderboard = SQL("SELECT * FROM leaderboards WHERE id = ?", $lbID);
        if($leaderboard == null)
        {
            echo 0;
            exit();
        }
        $loaded_leaderboard = new Leaderboard($leaderboard[0]);
        if($action == "1")
            $loaded_leaderboard->addBot($botID);
        else
            $loaded_leaderboard->removeBot($botID);
        echo "1";
    } else
        die("Invalid request");
} else
    die("Invalid request");