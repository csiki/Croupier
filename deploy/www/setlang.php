<?php
include "php/include.php";
if (isset($_GET["lang"]) && isValidLang($_GET["lang"])) {
    $_SESSION["lang"] = $_GET["lang"];
    if ($loggedin)
        SQL("UPDATE accounts SET lang = ? WHERE id = ?;", $_GET["lang"], $_SESSION["accountID"]);
}
header('Location: ' . $_SERVER['HTTP_REFERER']);