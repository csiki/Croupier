<?php
include "php/include.php";
needLogin();
$gameID = $botID = 0;
$date = $botName = "";
if (isset($_GET["gameID"]) && is_numeric($_GET["gameID"]) && isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $gameID = $_GET["gameID"];
    $botID = $_GET["botID"];
    //check game exits
    $log = SQL("SELECT endTime, log FROM games WHERE id = ? && checked = 1", $gameID);
    if ($log == null)
        die("Invalid request");
    $logFile = _LOG_RELATIVE_PATH_.$log[0]["log"];
    $date = $log[0]["endTime"];
    //check bot exits
    $bot = SQL("SELECT name FROM bots WHERE accountID = ? && id = ?", $_SESSION["accountID"], $botID);
    if($bot == null)
        die("Invalid request");
    $botName = $bot[0]["name"];
    //check if bot is participated in the game
    if(SQL("SELECT gameID FROM games_by_bots WHERE botID = ? && gameID = ?", $botID, $gameID) == null)
        die("Invalid request");
} else
    die("Invalid request");
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2>
        <?php
        echo sprintf($tr["SHOW_GAME"], $botName, $date);
        ?>
    </h2>

    <div class="basicContainer">
        <table>
            <thead>
            <tr>
                <th>Who</th>
                <th>What</th>
                <th>Severity</th>
            </tr>
            </thead>
            <tbody>
            <?php
            if(file_exists($logFile))
            {
                $log = simplexml_load_file($logFile);
                foreach ($log->event as $event) {
                    echo '<tr>';
                    echo '<td>'.$event->logger. '</td>';
                    echo '<td>'.$event->msg. '</td>';
                    echo '<td>'.$event->severity. '</td>';
                    echo '</tr>';
                }
            }
            else
                echo "Fatal erro during game";
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