<?php
include "php/functions.php";
sec_session_start();
if(isset($_GET["lang"]) && isValidLang($_GET["lang"]))
    $_SESSION["lang"] = $_GET["lang"];
header('Location: ' . $_SERVER['HTTP_REFERER']);