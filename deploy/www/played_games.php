<?php
include "php/include.php";
needLogin();
$botID = 0;
$botName = "";
if (isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $botID = $_GET["botID"];
    $bot = SQL("SELECT name FROM bots WHERE accountID = ? && id = ?", $_SESSION["accountID"], $botID);
    if ($bot == null)
        die("Invalid request");
    $botName = $bot[0]["name"];
} else
    die("Invalid request");
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
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
<?php include "php/header.php"; ?>
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
                <th style="width: 200px"><?=$tr["DATE_TIME"]?></th>
                <th style="width: 30px"><?=$tr["OPERATIONS"]?></th>
            </tr>
            </thead>
            <tbody>
            <?php
                $games = SQL("SELECT gameID FROM games_by_bots WHERE botID = ?", $botID);
                for ($i = 0; $i < count($games); $i++) {
                    $gameDate = SQL("SELECT endTime FROM games WHERE id = ? && checked = 1", $games[$i]["gameID"]);
                    echo '<tr>';
                    echo '<td>' . $gameDate[0]["endTime"] . '</td>';
                    echo '<td style="cursor:pointer" onclick="document.location = \'show_game.php'
                        . '?botID='
                        . $botID
                        . '&gameID='
                        . $games[$i]["gameID"]
                        . '\';">
                    <a>' . $tr["SHOW"]. '</a></td>';
                    echo '</tr>';
                }
            ?>
            </tbody>
        </table>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>