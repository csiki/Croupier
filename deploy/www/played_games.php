<?php
require_once "php/include.php";
needLogin();
$botID = 0;
$botName = "";
if (isset($_GET["id"]) && is_numeric($_GET["id"])) {
    $botID = $_GET["id"];
    $bot = SQL("SELECT name FROM bots WHERE id = ? AND (accountID = ? OR ?)", $botID, $_SESSION["accountID"], $admin);
    if ($bot == null)
        die("Invalid request");
    $botName = $bot[0]["name"];
} else
    die("Invalid request");
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script>
        $(function () {
            if ($("#manageBotsTable tbody tr").length == 0) {
                addTableEmpty();
            }
        });

        function addTableEmpty() {
            var $row = $('<td colspan="6" style="text-align:center"><?=$tr["NO_GAMES_FOUND"]?></td>');
            $row.hide();
            $row.appendTo($("#manageBotsTable tbody"));
            $row.slideDown(200);
        }
    </script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2>
        <?php
        echo sprintf($tr["PLAYED_GAMES_FOR"], $botName);
        ?>
    </h2>

    <div class="basicContainer">
        <table id="manageBotsTable">
            <thead>
            <tr>
                <th><?= $tr["DATE_TIME"] ?></th>
                <th><?= $tr["LEADERBOARD"] ?></th>
                <th><?= $tr["OPERATIONS"] ?></th>
            </tr>
            </thead>
            <tbody>
            <?php
            $games = array();
            
            $games = SQL("SELECT games.id, games.leaderboard, games.endTime FROM games, games_by_bots
                WHERE games.id = games_by_bots.gameID and games_by_bots.botID = ? ORDER BY games.endTime DESC LIMIT 0,20", $botID);
            
            /*$gameIDs = SQL("SELECT gameID FROM games_by_bots WHERE botID = ?", $botID);
            for ($i = 0; $i < count($gameIDs); $i++) {
                $game = SQL("SELECT id, leaderboard, endTime FROM games WHERE id = ? AND checked = 1 ORDER BY endTime DESC",
                    $gameIDs[$i]["gameID"]);
                if ($game != null) {
                    $games[] = $game[0];
                }
            }*/
            if ($games != null) {
                /*usort($games, function ($a, $b) {
                    return $a['endTime'] < $b['endTime'];
                });*/
                for ($i = 0; $i < count($games); $i++) {
                    $lb = SQL("SELECT friendlyName FROM leaderboards WHERE tableName = ?", $games[$i]["leaderboard"]);
                    echo '<tr>';
                    echo '<td>' . $games[$i]["endTime"] . '</td>';
                    echo '<td>' . $lb[0]["friendlyName"] . '</td>';
                    echo '<td style="cursor:pointer" onclick="document.location = \'show_game.php'
                        . '?botID=' . $botID
                        . '&gameID=' . $games[$i]["id"]
                        . '\';"><div class="icon showGameIcon" title="' . $tr["SHOW"] . '"></div></td>';
                    echo '</tr>';
                }
            }
            ?>
            </tbody>
        </table>
    </div>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>