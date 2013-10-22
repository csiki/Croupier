<?php
include "php/include.php";
needLogin();
if (isset($_GET["botID"])) {
    $res = SQL("DELETE FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $_GET["botID"]);
    if($res == null)
        die("Invalid request"); //not found botID
    $files = glob(_BOT_AI_RELATIVE_PATH_ . $_GET["botID"] . "/" . '*', GLOB_MARK);
    foreach ($files as $file) {
        unlink($file);
    }
    rmdir(_BOT_AI_RELATIVE_PATH_ . $_GET["botID"] . "/");
    echo "1";
} else
    echo "0";