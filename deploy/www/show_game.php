<?php
include "php/include.php";
needLogin();

abstract class Severity
{
    const FATAL = 0; //self, other
    const ERROR = 1; //self
    const WARNING = 2; //self
    const NOTIFICATION = 3; //self, other (croupier only)
    const INFORMATION = 4; //self, other
    const VERBOSE = 5; //self
    const DEBUG = 6; //none
};

$orig_msg = array("receiveCard", "fold", "check", "call", "raise");
$hun_msg = array("Kapott kártya", "Eldobás", "Passzolás", "Megadás", "Emelés");

$gameID = $botID = 0;
$date = $botName = "";
if (isset($_GET["gameID"]) && is_numeric($_GET["gameID"]) && isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $gameID = $_GET["gameID"];
    $botID = $_GET["botID"];
    //check game exits
    $log = SQL("SELECT endTime, log FROM games WHERE id = ? AND checked = 1", $gameID);
    if ($log == null)
        die("Invalid request");
    $logFile = _LOG_RELATIVE_PATH_ . $log[0]["log"];
    $date = $log[0]["endTime"];
    //check bot exits
    $bot = SQL("SELECT name FROM bots WHERE accountID = ? AND id = ?", $_SESSION["accountID"], $botID);
    if ($bot == null)
        die("Invalid request");
    $botName = $bot[0]["name"];
    //check if bot is participated in the game
    if (SQL("SELECT gameID FROM games_by_bots WHERE botID = ? AND gameID = ?", $botID, $gameID) == null)
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

        <?php
        if (file_exists($logFile)) {
            ?>
            <table>
                <thead>
                <tr>
                    <th>#</th>
                    <th>Who</th>
                    <th>What</th>
                    <th>Severity</th>
                </tr>
                </thead>
                <tbody>
                <?php
                $log = simplexml_load_file($logFile);
                $i = 1;
                foreach ($log->event as $event) {
                    //skip hidden events
                    if ($event->severity == Severity::DEBUG
                        || ($event->logger != $botID &&
                            ($event->severity == Severity::ERROR
                                || $event->severity == Severity::WARNING
                                || $event->severity == Severity::VERBOSE)
                        )
                    )
                        continue;

                    //calculate color for each severity
                    if ($event->logger == 0)
                        $rgb = "#E6E7FF";
                    else
                        $rgb = "#FFFF" . dechex(170 / 6 * $event->severity);

                    echo '<tr style="background-color: ' . $rgb . '; '
                        . ($event->logger == $botID ? 'font-weight:700; ' : '') . '">';
                    echo '<td>' . $i . '</td>';
                    if ($event->logger == 0)
                        echo '<td>Croupier</td>';
                    else if ($event->logger == $botID)
                        echo '<td>' . $botName . '</td>';
                    else
                        echo '<td>' . 'bot ' . $event->logger . '</td>';
                    $i++;
                    echo '<td>' . str_replace($orig_msg, $hun_msg, $event->msg) . '</td>';
                    echo '<td>' . $event->severity . '</td>';
                    echo '</tr>';
                }
                ?>

                </tbody>
            </table>
        <?php
        } else
            echo "Fatal error during game";
        ?>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>