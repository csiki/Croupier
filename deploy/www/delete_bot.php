<?php
require_once "php/include.php";
require_once "php/leaderboard.php";
needLogin();

if (isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $botID = $_GET["botID"];
    $res = SQL("DELETE FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $botID);
    if ($res == null) //not found botID
    {
        echo "1";
        exit();
    }

    SQL("DELETE FROM games_by_bots WHERE botID = ?", $_GET["botID"]);

    $leaderboards = SQL("SELECT * FROM leaderboards");
    foreach ($leaderboards as $leaderboard) {
        $loaded_leaderboard = new Leaderboard($leaderboard);
        $loaded_leaderboard->removeBot($botID);
    }

    unlink(_BOT_AI_RELATIVE_PATH_ . $_SESSION['accountID'] . "/" . $botID . ".cpp");
    echo "1";
} else
    echo "0";