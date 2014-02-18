<?php
include_once("connect_db.php");
include_once("functions.php");

$botsUnchecked = SQL("SELECT * FROM bots WHERE state = 'processing'");
if ($botsUnchecked != null)
    foreach ($botsUnchecked as $bot) {

        $accountid = $bot['accountID'];
        $botid = $bot['id'];
		
		/// compile
		$src = "../../data/bots/" . $accountid . "/" . $botid;
		
		// concat create & destroy
		$create_destroy_typedefs = "\n" . 'extern "C" Bot* create(BotCommunicator* communicator, int id, std::string name, BotLanguage lang){return new ConcreteBot(communicator, id, name, lang);}extern "C" void destroy(Bot* bot){delete bot;}';
		file_put_contents($src, $create_destroy_typedefs, FILE_APPEND | LOCK_EX);
		
        $descriptorspec = array(
            0 => array("pipe", "r"), //stdin
            1 => array("pipe", "w"), //stdout
            2 => array("pipe", "w") //stderr
        );
        $command = "sh ../../exec/compileSO.sh ".$src.".cpp ".$src;
        $process = proc_open($command, $descriptorspec, $pipes, dirname(__FILE__), null);// compileSO indítása
        if (is_resource($process)) {
            fclose($pipes[0]);
            $stdout = stream_get_contents($pipes[1]);
            fclose($pipes[1]);
            $stderr = stream_get_contents($pipes[2]);
            fclose($pipes[2]);
            $return_val = proc_close($process);
        }
        $stderr = str_replace("'", '"', $stderr); //TODO: Ez minek?
		if ($return_val == 0) // non-zero returnnél van para
		{
			// get gameid
			SQL("INSERT INTO games(checked, leaderboard, rules, log, result, startTime, endTime)
			VALUES(0, 'test', 'testrules.xml', '', '', 0, 0)");
			$gameid = $mysqli->insert_id;
			$resultname = $logname = $gameid . '.xml';
			SQL("UPDATE games SET log = ? , result = ? WHERE id = ?", $logname, $resultname, $gameid);

			$name = $bot['name'];
			$lang = $bot['code_lang'];
			$numofktables = 0; // for simplicity
			$ktables = array();
			$testcase = 0;

			$args = $accountid . ' ' . $botid . ' ' . $testcase . ' ' . $gameid . ' ' .
				$name . ' ' . $src . ' ' . $lang . ' ' . $numofktables . ' ' . explode(' ', $ktables);
            $process = proc_open("../../exec/bottester $args", $descriptorspec, $pipes, dirname(__FILE__), null);// compileSO indítása
            if (is_resource($process)) {
                fclose($pipes[0]);
                $stdout = stream_get_contents($pipes[1]);
                fclose($pipes[1]);
                $stderr = stream_get_contents($pipes[2]);
                fclose($pipes[2]);
                $return_val = proc_close($process);
            }
			echo $return_val;
			if ($return_val == 0) {
				// run gamemodule
				$command = "../../exec/gamemodule " . $gameid;
                $stderr = "";
				$return_val = 0;

				SQL("UPDATE games SET startTime = ? WHERE id = ?", time(),  $this->gameid);
                $process = proc_open($command, $descriptorspec, $pipes, dirname(__FILE__), null);// compileSO indítása
                if (is_resource($process)) {
                    fclose($pipes[0]);
                    $stdout = stream_get_contents($pipes[1]);
                    fclose($pipes[1]);
                    $stderr = stream_get_contents($pipes[2]);
                    fclose($pipes[2]);
                    $return_val = proc_close($process);
                }
				SQL("UPDATE games SET endTime = ? WHERE id = ?", time(), $this->gameid);

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
							SQL("UPDATE bots SET state = 'ok', runError = '0' WHERE id = ?", $botid);
						} else {
							SQL("UPDATE bots SET state = 'runtime', runError = ? WHERE id = ?", $gameid,  $botid);
						}
					}
				} else {
					SQL("UPDATE bots SET state='runtime', runError = ? WHERE id = ?", $gameid, $botid);
				}
			}
		}
		else {
			SQL("UPDATE bots SET state = 'compilation', compError = ? WHERE id = ?", $stderr, $botid);
		}
	}