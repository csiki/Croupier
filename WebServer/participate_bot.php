<?php
include "php/include.php";
needLogin();
if (isset($_GET["botid"]) && isset($_GET["val"])) {
    //validate
    if (($_GET["val"] == "1" || $_GET["val"] == "0") && is_numeric($_GET["botid"])) {
        //get bot and add to leaderboard
        SQL("UPDATE bots SET participate = ? WHERE id = ? AND accountID = ?;", $_GET["val"], $_GET["botid"], $_SESSION["accountID"]);
        echo "1";
    } else
        echo "0";
} else
    echo "0";