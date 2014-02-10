<?php
include "connect_db.php";
include "functions.php";
include "leaderboard.php";

$num_of_leaderboards_to_update = 2;
$leaderboards = SQL("SELECT * FROM leaderboards WHERE activated = 1 ORDER BY lastRefresh ASC");

$i = 0;
foreach($leaderboards as $leaderboard)
{
	if ($i == $num_of_leaderboards_to_update)
		break;
	update_leaderboard($leaderboard);
	++$i;
}

function update_leaderboard($leaderboard)
{
	$loaded_leaderboard = new Leaderboard($leaderboard);
	$loaded_leaderboard->randMatchmaking($leaderboard->getBotNum(), 2);
	$loaded_leaderboard->randMatchmaking($leaderboard->getBotNum(), 4);
	$loaded_leaderboard->randMatchmaking($leaderboard->getBotNum(), 8);
}

//get leaderboards
$leaderboards = SQL("SELECT * FROM leaderboards WHERE activated = 1 ORDER BY lastRefresh ASC");
foreach($leaderboards as $leaderboard)
{
    $tableName = $leaderboard["tableName"];
    $rulzFile = $leaderboard["rules"];
    //get the next game id
    $gameID = SQL("SHOW TABLE STATUS LIKE 'games'")[0]["Auto_increment"];
    //get bot ids from the leaderboard
    $botIDs = SQL("SELECT botID FROM ".$tableName." ORDER BY (win+loose) ASC LIMIT 6");
    if($botIDs == null)
        continue;
    //make game.xml
    $xmlRoot = new SimpleXMLElement('<?xml version="1.0"?><game></game>');
    $xmlRoot->addChild("id", $gameID);
    $xmlRoot->addChild("log", $gameID.".xml");
    $xmlRoot->addChild("results", $gameID.".xml");
    $botsElement = $xmlRoot->addChild("bots");
    //add bots to gameXML
    foreach($botIDs as $botID)
    {
        $bot = SQL("SELECT id, accountID, name, code_lang FROM bots WHERE id = ?", $botID["botID"]);
        $botElement = $botsElement->addChild("bot");
        $botElement->addChild("id", $botID["botID"]);
        $botElement->addChild("playerid", $bot[0]["accountID"]);
        $botElement->addChild("name", $bot[0]["name"]);
        $botElement->addChild("src", $botID["botID"]);
        $botElement->addChild("lang", getCodeLangID($bot[0]["code_lang"]));
        $botElement->addChild("credit", "2000");
        $botElement->addChild("knowledgetables");
    }
    $gameXML = "../" . _GAME_DATA_RELATIVE_PATH_ . $gameID . ".xml";
    //write to file formatted
    $dom = new DOMDocument('1.0');
    $dom->preserveWhiteSpace = false;
    $dom->formatOutput = true;
    $dom->loadXML($xmlRoot->asXML());
    $dom->save($gameXML);

    SQL("INSERT INTO games (id, checked, leaderboard, rules, log, result, startTime, endTime)
        VALUES (NULL, 0, ?, ?, ?, ?, NOW(), NOW()-1000)", $leaderboard["tableName"], $rulzFile,  $gameID.".log", $gameID.".xml");
    exec("cd .. && cd .. && cd exec && ./gamemodule " . $gameID . ".xml"); //Maximum execution time of 30 sec
    SQL("UPDATE leaderBoards SET lastRefresh = NOW() WHERE tableName = ?", $tableName);
}