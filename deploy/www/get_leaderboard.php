<?php
include_once "php/include.php";
needLogin();
function getLeaderboardForUser($lbID)
{
    $leaderboard = SQL("SELECT tableName FROM leaderboards WHERE id = ?", $lbID);
    if($leaderboard == null)
    {
        return null;
    }
    $tableName = $leaderboard[0]["tableName"];
   $result = SQL("SELECT name, username, score
                        FROM bots
                        INNER JOIN $tableName on $tableName.botID = bots.id
                        INNER JOIN accounts on accounts.id = bots.accountID
                        ORDER BY $tableName.score DESC");
    for($i = 0; $i < count($result); $i++)
    {
        $result[$i]["score"] = number_format(floatval($result[$i]["score"]), 0, '', '');
    }
    return $result;
}

if(isset($_GET["leaderboardID"]) && is_numeric($_GET["leaderboardID"]))
{
    echo json_encode(getLeaderboardForUser($_GET["leaderboardID"]));
}
else
    echo 'null';
?>