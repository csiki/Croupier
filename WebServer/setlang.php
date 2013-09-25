<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Levys
 * Date: 2013.09.25.
 * Time: 21:34
 * To change this template use File | Settings | File Templates.
 */
include "php/functions.php";
sec_session_start();
if(isset($_GET["lang"]) && isValidLang($_GET["lang"]))
    $_SESSION["lang"] = $_GET["lang"];
header('Location: ' . $_SERVER['HTTP_REFERER']);