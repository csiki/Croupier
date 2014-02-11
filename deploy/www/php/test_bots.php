<?php
include_once("connect_db.php");
include_once("functions.php");

$botsUnchecked = SQL("SELECT * FROM bots WHERE state = 'processing'");
if ($botsUnchecked != null)
    foreach ($botsUnchecked as $bot) {

        $accountid = $bot['accountID'];
        $botid = $bot['id'];
		
		// compile
		$src = "../../data/bots/" . $accountid . "/" . $botid;
		exec("../../exec/compileSO.sh ".$src.".cpp ".$src, $output, $return_val); // compileSO indítása
		$output = string_replace("'", '"', $output);
		
		if ($return_val == 0) // non-zero returnnél van para
		{
			// get gameid
			SQL("INSERT INTO games(checked, leaderboard, rules, log, result, startTime, endTime)
			VALUES(0, 'test', 'testrules.xml', '', '', 0, 0)");
			$gameid = $mysqli->insert_id;
			$resultname = $logname = $gameid . '.xml';
			SQL("UPDATE games SET log='$logname', result='$resultname'
			WHERE id=" . $gameid);

			$name = $bot['name'];
			$lang = $bot['code_lang'];
			$numofktables = 0; // for simplicity
			$ktables = array();
			$testcase = 0;

			$output = array();
			$return_val = 0;
			$args = $accountid . ' ' . $botid . ' ' . $testcase . ' ' . $gameid . ' ' .
				$name . ' ' . $src . ' ' . $lang . ' ' . $numofktables . ' ' . explode(' ', $ktables);
			exec("../../exec/bottester $args", $output, $return_val);
			echo $return_val;
			if ($return_val == 0) {
				// run gamemodule
				$command = "../../exec/gamemodule " . $gameid;
				$output = "";
				$return_val = 0;

				SQL("UPDATE games SET startTime='" . time() . "'
				WHERE id=" . $this->gameid);
				exec($command, $output, $return_val);
				SQL("UPDATE games SET endTime='" . time() . "'
				WHERE id=" . $this->gameid);

				if ($return_val == 4) // everything went alright
				{
					// still need to iterate through log,
					// to check if there's any warning or error on the tested bot
					$alrighty = true;
					if (file_exists('../../data/logs/' . $gameid . '.xml')) {
						$xml = simplexml_load_file('../../data/logs/' . $gameid . '.xml');

						foreach ($xml->log->event as $event) {
							if ((int)$event->logger == $botid && $event->severity == 1) {
								// error found
								$alrighty = false;
								break;
							}
						}

						if ($alrighty) {
							SQL("UPDATE bots SET state='ok' WHERE id=" . $botid);
							SQL("UPDATE bots SET runerror='0' WHERE id=" . $botid);
						} else {
							SQL("UPDATE bots SET state='runtime' WHERE id=" . $botid);
							SQL("UPDATE bots SET runerror='" . $gameid . "' WHERE id=" . $botid);
						}
					}
				} else {
					SQL("UPDATE bots SET state='runtime' WHERE id=" . $botid);
					SQL("UPDATE bots SET runerror='" . $gameid . "' WHERE id=" . $botid);
				}
			}
		}
		else {
			SQL("UPDATE bots SET state='compilation' WHERE id=" . $botid);
			SQL("UPDATE bots SET comperror='" . $output . "' WHERE id=" . $botid);
		}
	}