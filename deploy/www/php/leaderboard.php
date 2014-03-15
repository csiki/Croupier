<?php

class Leaderboard
{
    private $tableName;
    private $rulzName;
    private $gameid;
    private $bots = array();

    function __construct($leaderboard)
    {
        $this->tableName = $leaderboard["tableName"];
        $this->rulzName = $leaderboard["rules"];
        $this->bots = SQL("SELECT botID, score, win, loose FROM " . $this->tableName . " ORDER BY (win+loose)");
    }

    public function getBotNum()
    {
        return count($this->bots);
    }

    public function randMatchmaking($nGames, $nPlayers)
    {
        $sumWeight = 0;

        for ($i = 0; $i < count($this->bots); ++$i) {
            if ($this->bots[$i]["win"] + $this->bots[$i]["loose"] != 0)
                $sumWeight += 1.0 / ($this->bots[$i]["win"] + $this->bots[$i]["loose"]);
            else $sumWeight += 1.0;
        }

        $randomWeights = array();
        $players = array();

        for ($i = 0; $i < $nGames; ++$i) {
            // clear arrays
            unset($randomWeights);
            unset($players);
            $randomWeights = array();
            $players = array();

            // rand nums
            for ($p = 0; $p < $nPlayers; ++$p) {
                array_push($randomWeights, mt_rand() / mt_getrandmax() * $sumWeight);
            }
            sort($randomWeights);

            // find players accorording to their weights (how much they've played already)
            $b = 0;
            $r = 0;
            $i2 = 0;
            $weight = 0;
            $nBotsMultipleSelected = 0;
            while ($b < $this->getBotNum() && $r < $nPlayers) {
                if ($this->bots[$b]["win"] + $this->bots[$b]["loose"] != 0)
                    $weight += 1.0 / ($this->bots[$b]["win"] + $this->bots[$b]["loose"]);
                else $weight += 1.0;

                if ($randomWeights[$r] < $weight) {
                    array_push($players, $this->bots[$b]);
                    ++$r;

                    // if a player selected more than once, we random another later
                    $i2 = $r;
                    if ($i2 < $nPlayers) {
                        while ($i2 < $nPlayers && $randomWeights[$i2] < $weight) {
                            ++$nBotsMultipleSelected;
                            ++$i2;
                        }
                    }
                    $r = $i2;
                }
                ++$b;
            }

            // add alternatives for previously multiple selected bots
            $tmpbot = array();
            for ($i3 = 0; $i3 < $nBotsMultipleSelected; ++$i3) {
                do {
                    $tmpbot = $this->bots[mt_rand(0, count($this->bots) - 1)];
                } while (in_array($players, $tmpbot));
            }
            // play match
            $return_var = $this->playMatch($players);

            // process results
            $this->processResults($return_var);
        }
    }

    protected function playMatch($players)
    {
        global $mysqli;
        // insert database row into games
        // and retreive the id of thr row
        SQL("INSERT INTO games(checked, leaderboard, rules, log, result, startTime, endTime)
			VALUES(0, '" . $this->tableName . "', '" . $this->rulzName . "', '', '', NOW(), 0)");
        $this->gameid = $mysqli->insert_id;
        $resultname = $logname = $this->gameid . '.xml';
        SQL("UPDATE games SET log='$logname', result='$resultname'
			WHERE id=" . $this->gameid . "");

        // prepare gamedata xml
        $xmlRoot = new SimpleXMLElement('<?xml version="1.0"?><game></game>');
        $xmlRoot->addChild("id", $this->gameid);
        $xmlRoot->addChild("log", $logname);
        $xmlRoot->addChild("rules", $this->rulzName);
        $xmlRoot->addChild("results", $resultname);
        $botsElement = $xmlRoot->addChild("bots");

        //add bots to gameXML and update games_by_bots
        foreach ($this->bots as $bot) {
            // games_by_bots update
            SQL("INSERT INTO games_by_bots(gameID, botID) VALUES(" . $this->gameid . ", " . $bot["botID"] . ")");

            // xml
            $botdata = SQL("SELECT * FROM bots WHERE id = " . $bot["botID"]);
            $botElement = $botsElement->addChild("bot");
            $botElement->addChild("id", $bot["botID"]);
            $botElement->addChild("playerid", $botdata[0]["accountID"]);
            $botElement->addChild("name", $botdata[0]["name"]);
            $botElement->addChild("src", $botdata[0]["accountID"] . "/" . $bot["botID"]);
            $botElement->addChild("lang", getCodeLangID($botdata[0]["code_lang"]));
            $botElement->addChild("credit", 2000); // 2000 konstans !!!

            // knowledge tables
            $ktablesElement = $botElement->addChild("knowledgetables");
            $ktables = array();
            /*
            $ktables = SQL("SELECT id FROM knowledge WHERE ownerID = ? AND used = 1", $botdata[0]["accountID"]);
            if ($ktables != null) {
                foreach ($ktables as $ktable) {
                    $ktablesElement->addChild("tableid", $ktable);
                }
            }*/
        }
        $gameXML = "../../data/games/" . $this->gameid . ".xml";

        //write to file formatted
        $dom = new DOMDocument('1.0');
        $dom->preserveWhiteSpace = false;
        $dom->formatOutput = true;
        $dom->loadXML($xmlRoot->asXML());
        echo "\n" . $dom->save($gameXML) . "bytes\n";

        // run gamemodule
        $command = "./gamemodule " . $this->gameid;
        $return_val = 0;
        $descriptorspec = array(
            0 => array("pipe", "r"), //stdin
            1 => array("pipe", "w"), //stdout
            2 => array("pipe", "w") //stderr
        );
        $process = proc_open($command, $descriptorspec, $pipes, "../../exec", null);// compileSO indítása
        if (is_resource($process)) {
            fclose($pipes[0]);
            $stdout = stream_get_contents($pipes[1]);
            fclose($pipes[1]);
            $stderr = stream_get_contents($pipes[2]);
            fclose($pipes[2]);
            $return_val = proc_close($process);
        }
        echo "stdout". "\n". $stdout . "\n";
        echo "stderr". "\n" . $stderr . "\n";
        echo "gamemodule returned: " . $return_val . "\n";

        return $return_val;
    }

    protected function processResults($return_var)
    {
        if ($return_var == 4) // everything went alright
        {
            SQL("UPDATE games SET endTime = NOW() WHERE id = " . $this->gameid);
            // read results xml
            try {
                $result = new SimpleXMLElement("../../data/results/" . $this->gameid . '.xml', 0, true);
            } catch (Exception $e) {
                return;
            }
            // find winner
            $winnerid = 0;
            foreach ($result->bot as $botres) {
                if ($botres->kickatround == 0) // winner
                {
                    $winnerid = $botres->botid;
                    break;
                }
            }

            // looser score update
            $res = SQL("SELECT score FROM " . $this->tableName . " WHERE botID=$winnerid");
            $Rw = $res[0]["score"];
            $Rwdiff = 0;
            foreach ($result->bot as $botres) {
                if ($winnerid != $botres->botid) {
                    $Rwdiff += $this->updateScores($winnerid, $botres->botid);
                }
            }
			
			// winner score update
            $Rw += $Rwdiff;
            SQL("UPDATE " . $this->tableName . " SET score=$Rw WHERE botID=$winnerid");
			
			// update check
			SQL("UPDATE games SET checked = 1 WHERE id = " . $this->gameid);
        }
    }

    protected function updateScores($winnerid, $looserid)
    {
        $res = SQL("SELECT score FROM " . $this->tableName . " WHERE botID=$winnerid");
        $Rw = $res[0]["score"];
        $res = SQL("SELECT score FROM " . $this->tableName . " WHERE botID=$looserid");
        $Rl = $res[0]["score"];
        $Rwdiff = 0;
        if ($Rw > 0 && $Rl > 0) {
            // elo rating system: http://en.wikipedia.org/wiki/Elo_rating_system#Mathematical_details
            $Ew = 1.0 / (1.0 + pow(10.0, ($Rl - $Rw) / 400.0));
            //$El = 1.0 / (1.0 + pow(10.0, ($Rw - $Rl) / 400.0));

            $K = 24;
            $Rwdiff = (1.0 - $Ew) * $K;
            $Rl -= (1.0 - $Ew) * $K;

            SQL("UPDATE " . $this->tableName . " SET score=$Rl WHERE botID=$looserid");
            // winner update later
        }

        return $Rwdiff;
    }

    public function addBot($botID)
    {
        $res = SQL("SELECT state FROM bots WHERE id = ?", $botID);
        if($res == null || $res[0]["state"] != "ok")
            die("Invalid request");
        SQL("INSERT INTO ".$this->tableName." (botId) values(?)", $botID);
        SQL("INSERT INTO ".$this->tableName."_yesterday"." (botId) values(?)", $botID);
    }

    public function removeBot($botID)
    {
        SQL("DELETE FROM ".$this->tableName." WHERE botId = ?", $botID);
        SQL("DELETE FROM ".$this->tableName."_yesterday"." WHERE botId = ?", $botID);
    }
}
;

?>