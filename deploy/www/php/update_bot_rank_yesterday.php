<?php
require "connect_db.php";
require "functions.php";

/*
rank_yesterday t�bla:
	botID, rank, score
*/

$leaderboard_names = SQL("SELECT tableName FROM leaderboards WHERE activated=1");

foreach ($leaderboard_names as $leaderboard_name)
{
	SQL("CREATE TABLE IF NOT EXISTS " . $leaderboard_name . "_yesterday (botID int(11) NOT NULL PRIMARY_KEY, rank int(12) NOT NULL, score float NOT NULL)");
	SQL("TRUNCATE TABLE " . $leaderboard_name . "_yesterday");
	
	$bots = SQL("SELECT * FROM " . $leaderboard_name . " ORDER BY score DESC");
	$i = 1;
	foreach ($bots as $bot)
	{
		SQL("INSERT INTO " . $leaderboard_name . "_yesterday (botID, rank, score) VALUES (".$bot['botID'].", $i, ".$bot['score'].")");
		++$i;
	}
}
?>