<?php
require_once "php/include.php";
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

$severityColors = array(
    0 => "#000000",
    1 => "#CC0000",
    2 => "#FF8200",
    3 => "#E6E7FF",
    4 => "#f3f3ff",
    5 => "#FFFFFF"
);

$orig_msg = array("receiveCard", "fold", "check", "call", "raise",
	"letsPoker", "rmDealerButton", "addDealerButton", "burn", "betRound", "quit", "dealing",
    "preflop", "flop", "turn", "river", "revealCards","handOutPot",
	"collectCards", "roundStarted ", "roundEnded", "refreshBlinds", "showdown", "roundWinners");
$hun_msg = array("Kapott kártya", "Eldob", "Passzol", "Megad", "Emel",
	"Indul a játék", "Dealer gombot eltávolítja", "Dealer gombot hozzáadja", "Éget", "Tét kör indul", "Kilép",
    "Osztás", "PreFlop", "Flop", "Turn:", "River:", "Kártyáit megmutatja:", "Tétet vitte:",
	 "Kártyákat begyüjti", "Kör kezdődik #", "Véget ért a kör", "Vakok frissítése", "Showdown", "Nyertesek a körben:");
$en_msg = array("Receive Card", "Fold", "Check", "Call", "Raise",
    "Poker starts", "Dealer removes button", "Dealer adds button", "Burn", "Bet round starts", "Quit",
    "Dealing", "PreFlop", "Flop", "Turn:", "River:", "Shows cards:", "Pot taken by:",
    "Collects cards", "Round started #", "Round ended", "Refesh blinds", "Showdown", "Winners in round:");
    
// cards
$cardnames = array();
$cardpics = array();
$suits = array("HEARTS", "DIAMONDS", "CLUBS", "SPADES", "NULL");
$ranks = array("DEUCE of ", "THREE of ", "FOUR of ", "FIVE of ", "SIX of ", "SEVEN of ", "EIGHT of ",
    "NINE of ", "TEN of ", "JACK of ", "QUEEN of ", "KING of ", "ACE of ", "NULL of ");
$CWIDTH = 125;
$CHEIGHT = 181; // in px

for ($s = 0; $s < count($suits) - 1; ++$s) // null not included
{
    for ($r = 0; $r < count($ranks) - 1; ++$r)
    {
        $cname = $ranks[$r] . $suits[$s];
        $margin = -$s * $CHEIGHT * 0.4 . 'px 0 0 ' . -$r * $CWIDTH * 0.4 . 'px';
        $cropped_image = '<div style="width: 50px; height: 72px; overflow:hidden;"><img src="images/cards.png" alt="'.$cname.'" style="width:650px; height:362px; margin: '.$margin.'" /></div>';
        
        array_push($cardnames, $cname);
        array_push($cardpics, $cropped_image);
    }
}
array_push($cardnames, 'NULL of NULL');
array_push($cardpics, '<div style="width: 50px; height: 72px; overflow:hidden;"><img src="images/cards.png" alt="NO INFORMATION" style="width:650px; height:362px; margin: 724px 0 0 0;" />');


$gameID = $botID = 0;
$date = $botName = "";
if (isset($_GET["gameID"]) && is_numeric($_GET["gameID"]) && isset($_GET["botID"]) && is_numeric($_GET["botID"])) {
    $gameID = $_GET["gameID"];
    $botID = $_GET["botID"];
    //check game exits
    $log = SQL("SELECT endTime, log FROM games WHERE id = ? AND checked = 1", $gameID);
    if ($log == null)
        die("Invalid request |||");
    $logFile = _LOG_RELATIVE_PATH_ . $log[0]["log"];
    $date = $log[0]["endTime"];
    //check bot exits
    $bot = SQL("SELECT name FROM bots WHERE id = ? AND (accountID = ? OR ?)",$botID, $_SESSION["accountID"], $admin);
    if ($bot == null)
        die("Invalid request - bot not exits");
    $botName = $bot[0]["name"];
    //check if bot is participated in the game
    if (SQL("SELECT gameID FROM games_by_bots WHERE botID = ? AND gameID = ?", $botID, $gameID) == null)
        die("Invalid request - bot not participated");
} else
    die("Invalid request ----");
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
                    <th><?=$tr["NAME"]?></th>
                    <th><?=$tr["ACTIVITY"]?></th>
                </tr>
                </thead>
                <tbody>
                <?php
                $log = simplexml_load_file($logFile);
                $i = 1;
                $otherBots = array();
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
                    $rgb = $severityColors[(int)$event->severity];
                    echo '<tr style="background-color: ' . $rgb . '; '
                        . ($event->logger == $botID ? 'font-weight:700; ' : '') . '">';
                    echo '<td>' . $i++ . '</td>';
                    if ($event->logger == 0)
                        echo '<td>Croupier</td>';
                    else if ($event->logger == $botID)
                        echo '<td>' . $botName . '</td>';
                    else
                    {
                        if(!array_key_exists((string)($event->logger), $otherBots))
                        {
                            $res = SQL("SELECT name FROM bots WHERE id = ?", (string)($event->logger));
                            if($res != null)
                                $otherBots[(string)($event->logger)] = $res[0]["name"];
                            else
                                $otherBots[$event->logger] = "bot " . (string)($event->logger);
                        }
                        echo '<td>' . $otherBots[(string)($event->logger)] . '</td>';
                    }
                       
                    $msg = ($_SESSION["lang"] == "hu") ?
                        str_replace($orig_msg, $hun_msg, $event->msg) :
                        str_replace($orig_msg, $en_msg, $event->msg);
                    
                    $msg = str_replace($cardnames, $cardpics, $msg);
                    
                    echo '<td>' . $msg . '</td>';
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
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>