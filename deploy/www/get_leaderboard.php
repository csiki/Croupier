<?php
include_once "php/include.php";
needLogin();
function getLeaderBoard($lb)
{
    $lbName = "leaderboard" . $lb;
    if ($result = SQL("SELECT bots.name, accounts.username, " . $lbName . ".score
                        FROM bots, " . $lbName . ", accounts
                        WHERE " . $lbName . ".botID = bots.id
                        AND accounts.id = bots.accountID
                        ORDER BY " . $lbName . ".score DESC")
    ) {
        if ($result != null)
            return $result;
    }
    return array();
}

if(isset($_GET["leaderBoard"]) && is_numeric($_GET["leaderBoard"]))
{
    echo json_encode(getLeaderBoard($_GET["leaderBoard"]));
}
?>