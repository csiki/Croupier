<?php
include "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["SUMMARY"] ?></h2>

    <h3><?= $tr["SUMMARY_WELCOME"] ?></h3>
    <?php

    echo "<table>";
    $leaderboards = SQL("SELECT tableName FROM leaderboards");
    foreach ($leaderboards as $lbName) {
        $bots = SQL("SELECT botID, score, name FROM " . $lbName["tableName"] . " INNER JOIN bots INNER JOIN accounts" .
            " ON " . $lbName["tableName"] . ".botID = bots.id AND bots.accountID = accounts.id AND accounts.id = ?", $_SESSION["accountID"]);
        $botsOld = SQL("SELECT botID, score FROM " . $lbName["tableName"] . "_yesterday" .
            " INNER JOIN bots ON " . $lbName["tableName"] . "_yesterday" . ".botID = bots.id");

        if ($bots != null) {
            for ($i = 0; $i < count($bots); $i++) {
                if ($bots[$i]["score"] != $botsOld[$i]["score"]) {
                    echo "<tr>";
                    echo "<td>" . ($bots[$i]["score"] < $botsOld[$i]["score"] ? "↓" : "↑") . "</td>";
                    echo "<td>" . $bots[$i]["score"] . "</td>";
                    echo "<td>" . $bots[$i]["name"] . "</td>";
                    echo "<td>" . $lbName["tableName"] . "</td>";
                    echo "</tr>";
                }
            }
        }
    }
    echo "</table>";
    ?>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>