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

    <h3><?= $tr["SUMMARY_LATEST_CHANGES"] ?></h3>
    <?php
    echo '<table>
            <thead>
                    <tr>
                        <th style = "width: 40px"></th>
                        <th style = "width: 80px"> ' . $tr["SCORE"] . '</th>
                        <th style = "width: 150px"> ' . $tr["NAME"] . '</th>
                        <th style = "width: 140px"> ' . $tr["LEADERBOARD"] . '</th>
                    </tr>
            </thead>
            <tbody>';
    $leaderboards = SQL("SELECT tableName FROM leaderboards");
    foreach ($leaderboards as $lbName) {
        $bots = SQL("SELECT botID, score, name FROM " . $lbName["tableName"] . " INNER JOIN bots INNER JOIN accounts" .
            " ON " . $lbName["tableName"] . ".botID = bots.id AND bots.accountID = accounts.id AND accounts.id = ?", $_SESSION["accountID"]);
        $botsOld = SQL("SELECT botID, score FROM " . $lbName["tableName"] . "_yesterday" .
            " INNER JOIN bots ON " . $lbName["tableName"] . "_yesterday" . ".botID = bots.id");

        if ($bots != null) {
            for ($i = 0; $i < count($bots); $i++) {
                if ($bots[$i]["score"] != $botsOld[$i]["score"]) {
                    $up = $bots[$i]["score"] > $botsOld[$i]["score"];
                    echo '<tr>';
                    echo '<td><div class="icon ' .($up ? 'upArrowIcon' : 'downArrowIcon').'"></div></td>';
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