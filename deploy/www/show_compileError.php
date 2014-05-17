<?php
require_once "php/include.php";
needLogin();
$botID = 0;
if (isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $botID = $_GET["botID"];
    $bot = SQL("SELECT name, compError FROM bots WHERE id = ? AND (accountID = ? OR ?) AND state = 'compilation'",
        $botID, $_SESSION["accountID"], $admin);
    if ($bot == null)
        die("Invalid request");
    $botName = $bot[0]["name"];
    $compError = $bot[0]["compError"];
} else
    die("Invalid request");
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2>
        <?php
        echo $tr["STATE_COMPILATION"];
        echo ' - ';
        echo $botName;
        ?>
    </h2>

    <div class="basicContainer">
        <textarea id="compileErrorTextarea" readonly><?=$compError?></textarea>
        <input type="button" onclick="window.history.back()" class="button" value="<?= $tr["BACK"] ?>">
    </div>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>