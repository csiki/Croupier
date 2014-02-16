<?php
require_once "connect_db.php";
require_once "functions.php";
require_once "leaderboard.php";

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