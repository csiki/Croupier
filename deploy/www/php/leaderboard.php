<?php

class Leaderboard
{
	private $tableName;
	private $rulzFile;
	private $bots = array();
	
	Leaderboard($leaderboard)
	{
		$tableName = $leaderboard["tableName"];
		$rulzFile = $leaderboard["rules"];
		$bots = SQL("SELECT botID, score, win, loose FROM ".$tableName." ORDER BY (win+loose)");
	}
	
	public function randMatchmaking($nGames, $nPlayers)
	{
		$sumWeight = 0;
		
		for ($i = 0; $i < count($bots); ++$i)
		{
			sumWeight += 1.0f / ($bots[$i]["win"] + $bots[$i]["loose"]);
		}
	}
};

?>