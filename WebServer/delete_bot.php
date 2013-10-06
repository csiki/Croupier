<?php
include "php/include.php";
needLogin();
if (isset($_GET["botid"])) {
    SQL("DELETE FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $_GET["botid"]);
    echo "1";
} else
    echo "0";