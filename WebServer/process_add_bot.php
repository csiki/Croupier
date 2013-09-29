<?php
include "php/include.php";
needLogin();
if ((isset($_POST["code"]) || isset($_FILES["codefile"])) && isset($_POST["lang"])) {
    $id = 0;
    $name = "";
    $code = "";
    $lang = "";
    if ($stmt = $mysqli->prepare("SELECT id FROM bots WHERE account_id = ? ORDER BY id DESC LIMIT 1;")) {
        $stmt->bind_param("d", $_SESSION["account_id"]);
        $stmt->execute();
        $stmt->store_result();
        $stmt->bind_result($id);
        $stmt->fetch();
        $stmt->close();
    }
    echo "getD: " + $id;
    $id++;
    if(isset($_POST["name"]))
        $name = $_POST["name"];
    else
        $name = "Unnamed bot ".$id;
    if(isset($_FILES["codefile"]) && $_FILES["codefile"]["error"] == 0)
    {
        if(strpos($_FILES["codefile"]["type"], "text/") !== 0)
        {

        }
        $code = file_get_contents($_FILES["codefile"]["tmp_name"]);
    }
    else
    {
        $code = $_POST["code"];
    }
    $lang = $_POST["lang"];
    if ($insert_stmt = $mysqli->prepare("INSERT INTO bots (id, account_id, name, add_time, code, code_lang, state) VALUES (NULL, ?, ?, NOW(), ?, ?, 'processing')")) {
        $insert_stmt->bind_param('ssss', $_SESSION["account_id"], $name, $code, $lang);
        $insert_stmt->execute();
        //header('Location: manage_bots.php?success=1');
    } else
        dieDb($mysqli);
}