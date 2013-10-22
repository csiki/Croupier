<?php
include "connect_db.php";
include "functions.php";

$botsUnchecked = SQL("SELECT * FROM bots WHERE state = 'processing'");

foreach($botsUnchecked as $bot)
{
    var_dump($bot);
    exec("../../exec/Test " . $gameID . ".xml");
}