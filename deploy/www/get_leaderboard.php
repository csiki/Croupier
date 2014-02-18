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

   $result = SQL("SELECT bots.name, accounts.username, " . $leaderboard[0]["tableName"] . ".score
                        FROM bots, " . $leaderboard[0]["tableName"] . ", accounts
                        WHERE " . $leaderboard[0]["tableName"] . ".botID = bots.id
                        AND accounts.id = bots.accountID
                        ORDER BY " . $leaderboard[0]["tableName"] . ".score DESC");

    return $result;
}

if(isset($_GET["leaderboardID"]) && is_numeric($_GET["leaderboardID"]))
{
    echo json_encode(getLeaderboardForUser($_GET["leaderboardID"]));
}
else
    echo 'null';
?>