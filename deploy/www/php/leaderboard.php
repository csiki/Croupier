<?php

class Leaderboard
{
	private $tableName;
	private $rulzName;
	private $resultName;
	private $gameid;
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
			$sumWeight += 1.0f / ($bots[$i]["win"] + $bots[$i]["loose"]);
		}
		
		$randomWeights = array();
		$players = array();
		
		for ($i = 0; $i < $nGames; ++$i)
		{
			// clear arrays
			unset($randomWeights);
			unset($players);
			$randomWeights = array();
			$players = array();
			
			// rand nums
			for ($p = 0; $p < nPlayers; ++$p)
			{
				array_push($randomWeights, mt_rand() / mt_getrandmax() * $sumWeight);
			}
			sort($randomWeights);
			
			// find players accorording to their weights (how much they've played already)
			$b = 0;
			$r = 0;
			$i = 0;
			$weight = 0;
			$nBotsMultipleSelected = 0;
			while ($b < count($bots) && $r < $nPlayers)
			{
				$weight += 1.0f / ($bots[$b]["win"] + $bots[$b]["loose"]);
				
				if ($randomWeights[$r] < $weight)
				{
					array_push($players, $bots[$b]);
					++$r;
					
					// if a player selected more than once, we random another later
					$i = $r;
					if ($i < $nPlayers)
					{
						while ($i < $nPlayers && $randomWeights[$i] < $weight)
						{
							++$nBotsMultipleSelected;
							++$i;
						}
					}
					$r = $i;
				}
				++$b;
			}
			
			// add alternatives for previously multiple selected bots
			$tmpbot = array();
			for ($i = 0; $i < $nBotsMultipleSelected; ++$i)
			{
				do {
					$tmpbot = $bots[mt_rand(0, count($bots) - 1)];
				} while (in_array($players, $tmpbot));
			}
			
			// play match
			$return_var = playMatch($players);
			
			// process results
			processResults($return_var);
		}
	}
	
	protected function playMatch($players)
	{
		global $mysqli;
		// insert database row into games
		// and retreive the id of thr row
		SQL("INSERT INTO games(checked, leaderboard, rules, log, result, startTime, endTime)
			VALUES(0, '$tableName', '$rulzName', '', '', 0, 0)");
		$gameid = $mysqli->insert_id;
		$resultName = $logname = $gameid.'.xml';
		SQL("UPDATE games SET log='$logname', result='$resultname'
			WHERE id=$gameid");
		
		// prepare gamedata xml
		$xmlRoot = new SimpleXMLElement('<?xml version="1.0"?><game></game>');
		$xmlRoot->addChild("id", $gameid);
		$xmlRoot->addChild("log", $logname);
		$xmlRoot->addChild("results", $resultname);
		$botsElement = $xmlRoot->addChild("bots");
		
		//add bots to gameXML
		foreach($bots as $bot)
		{
			$botdata = SQL("SELECT * FROM bots WHERE id = ".$bot["id"]);
			$botElement = $botsElement->addChild("bot");
			$botElement->addChild("id", $bot["id"]);
			$botElement->addChild("playerid", $botdata[0]["accountID"]);
			$botElement->addChild("name", $botdata[0]["name"]);
			$botElement->addChild("src", $botdata[0]["src"]);
			$botElement->addChild("lang", getCodeLangID($bot[0]["code_lang"]));
			$botElement->addChild("credit", 2000); // 2000 konstans !!!
			
			// knowledge tables
			$ktablesElement = $botElement->addChild("knowledgetables");
			$ktables = SQL("SELECT id FROM knowledge WHERE ownerID=".$botdata[0]["accountID"]." AND used=1");
			foreach ($ktable as $ktables)
			{
				$ktablesElement->addChild("tableid", $ktable);
			}
		}
		$gameXML = "../data/games/" . $gameid . ".xml";
		
		//write to file formatted
		$dom = new DOMDocument('1.0');
		$dom->preserveWhiteSpace = false;
		$dom->formatOutput = true;
		$dom->loadXML($xmlRoot->asXML());
		$dom->save($gameXML);
		
		// run gamemodule
		$command = "../exec/gamemodule $gameid";
		$output = "";
		$return_var = 0;
		exec($command, $output, $return_var);
		
		return $return_var;
	}
	
	public function processResults($return_var)
	{
		// read results xml
		$result = new SimpleXMLElement("../data/results/".$resultName);
		$kicks = array();
		foreach ($result->results->bot as $botres)
		{
			$kicks[$botres->botid] = $botres->kickatround;
		}
		arsort($kicks); // reverse sorting by {kick at round}
		
		foreach ($kicks as $winnerid => $winnerkick)
		{
			foreach ($kicks as $looserid => $looserkick)
			{
				// TODO növelni win-t és loose-t !!!
				if ($winnerid != $looserid && $winnerkick != $looserkick)
				{
					updateScores($winnerid, $looserid);
				}
				else if ($winnerid != $looserid && $winnerkick == $looserkick) // draw
				{
					updateScores($winnerid, $looserid);
					updateScores($looserid, $winnerid);
				}
			}
			unset($kicks[$winnerid]); // we don't need it anymore
		}
	}
	
	protected function updateScores($winnerid, $looserid)
	{
		$Rw = SQL("SELECT score FROM bots WHERE id=$winnerid")[0]["score"]; // TODO ide kell [0]["score"] ???
		$Rl = SQL("SELECT score FROM bots WHERE id=$looserid")[0]["score"];
		
		// elo rating system: http://en.wikipedia.org/wiki/Elo_rating_system#Mathematical_details
		$Ew = 1 / (1 + pow(10, ($Rl - $Rw) / 400));
		$El = 1 / (1 + pow(10, ($Rw - $Rl) / 400));
		
		$K = 32;
		$Rw += (1 - $Ew) * $K;
		$Rl -= (1 - $Ew) * $K; // TODO ez így jó e?
		
		SQL("UPDATE $tableName SET score=$Rw WHERE id=$winnerid");
		SQL("UPDATE $tableName SET score=$Rl WHERE id=$looserid");
	}
};

?>