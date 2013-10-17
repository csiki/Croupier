<?php
include "php/include.php";
needLogin();
if (isset($_GET["botID"])) {
    SQL("DELETE FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $_GET["botID"]);
    echo "1";
} else
    echo "0";