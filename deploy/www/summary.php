<?php
require_once "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["SUMMARY"] ?></h2>

    <h3><?= $tr["SUMMARY_WELCOME"] ?></h3>

    <?php
    $botCount = SQL("SELECT COUNT(*) FROM bots WHERE accountID = ?", $_SESSION["accountID"]);
    $leaderboards = SQL("SELECT tableName FROM leaderboards");
    foreach ($leaderboards as $lbName) {
        $bots = SQL("SELECT botID, score, name FROM " . $lbName["tableName"] . " INNER JOIN bots INNER JOIN accounts" .
            " ON " . $lbName["tableName"] . ".botID = bots.id AND bots.accountID = accounts.id AND accounts.id = ?", $_SESSION["accountID"]);
        $botsOld = SQL("SELECT botID, score FROM " . $lbName["tableName"] . "_yesterday" .
            " INNER JOIN bots ON " . $lbName["tableName"] . "_yesterday" . ".botID = bots.id");
        if ($bots != null) {
            $latestChanges = '<h3>' . $tr["SUMMARY_LATEST_CHANGES"] . '</h3>';
            $latestChanges .= '<table>';
            $latestChanges .= '<thead>
                    <tr>
                        <th style = "width: 40px"></th>
                        <th style = "width: 80px"> ' . $tr["SCORE"] . '</th>
                        <th style = "width: 150px"> ' . $tr["NAME"] . '</th>
                        <th style = "width: 140px"> ' . $tr["LEADERBOARD"] . '</th>
                    </tr>
            </thead>
            <tbody>';
            $count = 0;
            for ($i = 0; $i < count($bots); $i++) {
                if ($bots[$i]["score"] != $botsOld[$i]["score"]) {
                    $count++;
                    $up = $bots[$i]["score"] > $botsOld[$i]["score"];
                    $latestChanges .= '<tr>';
                    $latestChanges .= '<td><div class="icon ' . ($up ? 'upArrowIcon' : 'downArrowIcon') . '"></div></td>';
                    $latestChanges .= "<td>" . $bots[$i]["score"] . "</td>";
                    $latestChanges .= "<td>" . $bots[$i]["name"] . "</td>";
                    $latestChanges .= "<td>" . $lbName["tableName"] . "</td>";
                    $latestChanges .= "</tr>";
                }
            }
            $latestChanges .= "</table>";
            if($count != 0)
                echo $latestChanges;
        }
    }
    if($botCount[0]["COUNT(*)"] == 0)
    {
        echo '<h3>'.$tr['SUMMARY_NO_BOTS'].'</h3>';
        echo '<a href="add_bot.php" class="button">'.$tr["CREATE_BOT"].'</a>';
    }
    ?>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>