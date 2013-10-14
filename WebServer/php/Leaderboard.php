<?php
class Leaderboard
{
    public $id;
    public $rulz = null;
    public $score = 0;
    public $win = 0;
    public $loose = 0;
    public $botsByPosition = null;

    public function addBot(Bot $bot)
    {
        $this->botsByPosition[] = $bot;
    }

    public function removeBot(Bot &$bot)
    {
        for ($i = 0; $i < count($this->botsByPosition); $i++) {
            if ($this->botsByPosition[$i] === $bot) {
                array_splice($this->botsByPosition, $i, 1);
                break;
            }
        }
    }

    public function getNthBot($index)
    {
        return $this->botsByPosition[$index];
    }

    public function executeMatches()
    {

    }

    public function Serialize()
    {
        //remove previous values
        SQL("DELETE FROM leaderboards_bots WHERE leaderboardID = ?", $this->id);
        SQL("DELETE FROM leaderboards WHERE id = ?", $this->id);
        //add new values
        SQL("INSERT INTO leaderboards (id, rulesID ) VALUES (NULL , ?);", $this->rulz->id);
        for ($i = 0; $i < count($this->botsByPosition); $i++)
            SQL("INSERT INTO leaderboards_bots (leaderboardID, botID, botRank) VALUES (?, ?, ?);",
                $this->id, $this->botsByPosition[$i]->id, $i);
    }

    public static function DeSerialize($id)
    {
        $ret = new Leaderboard();
        $result = SQL("SELECT * FROM leaderboards WHERE id = ?", $id);
        if($result == null)
            dieDb();
        $ret->id = $result[0]["id"];
        return $ret;
    }
}