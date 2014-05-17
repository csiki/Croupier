<?php
include_once "php/include.php";
needLogin();

function getBotStateText($state, $botID)
{
    global $tr;
    switch ($state) {
        case 'ok':
            return $tr["STATE_OK"];
        case 'processing':
            return $tr["STATE_PROCESSING"];
        case 'compilation':
            return "<a href=\"show_compileError.php?botID=" . $botID . "\">" . $tr["STATE_COMPILATION"] . "</a>";
        case 'runtime':
            return $tr["STATE_RUNTIME"];
    }
}

if(isset($_GET["botID"]) && is_numeric($_GET["botID"]))
{
    $botID = $_GET["botID"];
    $bot = SQL("SELECT state FROM bots WHERE id = ? AND accountID = ?", $botID, $_SESSION["accountID"]);
    if($bot == null)
        echo 'null';
    else
    {
        echo json_encode([$bot[0]["state"] != "processing" , getBotStateText($bot[0]["state"], $botID)]);
    }
}
else
    echo 'null';
?>