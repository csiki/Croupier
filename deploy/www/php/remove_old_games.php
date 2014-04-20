<?php
require_once("include.php");
date_default_timezone_set("Europe/Budapest");
$deltaDay = 6;
$date = new DateTime();
$date->sub(new DateInterval('P' . $deltaDay . 'D'));
$oldGames = SQL("SELECT id, log, result FROM games WHERE endTime < ?", $date->format("Y-m-d H:i:s"));
SQL("DELETE FROM games WHERE endTime < ?", $date->format("Y-m-d H:i:s"));
if($oldGames != null)
    foreach ($oldGames as $row) {
        SQL("DELETE FROM games_by_bots WHERE gameID = ?", $row['id']);
        unlink("../". _LOG_RELATIVE_PATH_ . $row["log"]);
        echo "../". _LOG_RELATIVE_PATH_ . $row["log"] . " deleted.\n";
        unlink("../". _RESULTS_RELATIVE_PATH_ . $row["result"]);
        echo "../". _RESULTS_RELATIVE_PATH_ . $row["result"] . " deleted.\n";
        unlink("../". _GAME_DATA_RELATIVE_PATH_ . $row["id"]. ".xml");
        echo "../". _GAME_DATA_RELATIVE_PATH_ . $row["id"]. ".xml" . " deleted.\n";
    }