<?php
include "php/include.php";
needLogin();
if (!isset($_GET["botid"]))
    die("Invalid request");
$botID = $_GET["botid"];
$name = $add_time = $code = $code_lang = $state = "";
if ($stmt = $mysqli->prepare("SELECT name, add_time, code, code_lang, state
    FROM bots WHERE account_id = ? AND id = ?")
) {
    $stmt->bind_param('ii', $_SESSION["account_id"], $botID);
    $stmt->execute();
    $stmt->store_result();
    if ($stmt->num_rows == 0)
        die("Invalid requessst");
    $stmt->bind_result($name, $add_time, $code, $code_lang, $state);
    $stmt->fetch();
} else dieDb($mysqli);
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$name?></h2>
    <br />
    <textarea cols="80" rows="20"style="display: block" wrap="off" readonly="true"><?=$code?></textarea>
</div>
<div id="footer"></div>
</body>
</html>