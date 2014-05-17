<?php
require_once "php/include.php";
require_once "php/leaderboard.php";
ob_start();
require_once "bot_state.php";
ob_end_clean();
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script>
        var compiledBotCount = 0;
        $(function () {
            $("#manageBotsTable tbody tr").on('remove', function () {
                if ($("#manageBotsTable tbody tr").length == 0) {
                    addTableEmpty();
                }
            });

            if ($("#manageBotsTable tbody tr").length == 0) {
                addTableEmpty();
            }

            setInterval(function () {
                checkBotsCompiled();
            }, 2000);
        });

        function checkBotsCompiled() {
            $("#manageBotsTable tbody tr").each(function (index) {
                var $botTR = $(this);
                if ($botTR.attr("data-compiling") == "true") {
                    var botID = $(this).attr("data-botID");
                    $.get("bot_state.php?botID=" + botID, function (data) {
                        var stateData = JSON.parse(data);
                        if (stateData[0] == true) {
                            messageBox("<?= $tr["BOT_COMPILED"] ?> " + $botTR.children(".botName").html());
                            $botTR.attr("data-compiling", "false");
                            $botTR.children(".botState").html(stateData[1]);
                        }
                    });
                }
            });
        }

        function addTableEmpty() {
            var $row = $('<td colspan="6" style="text-align:center"><?=$tr["NO_BOTS_FOUND"]?></td>');
            $row.hide();
            $row.appendTo($("#manageBotsTable tbody"));
            $row.slideDown(200);
        }

        function deleteBotAsk(element) {
            messageBoxYesNo("<?=$tr["DELETE_BOT_CONF"]?>", "<?=$tr["YES"]?>", "<?=$tr["NO"]?>", function () {
                addLoading(element)
                deleteBot(element);
            });
        }

        function deleteBot(element) {
            var botID = $(element).parent().attr("data-botID");
            $.get("delete_bot.php?botID=" + botID, function (data) {
                $(element).parent().children('td')
                    .animate({ 'padding-top': 0, 'padding-bottom': 0}, 100, "linear", function () {
                        $(this)
                            .wrapInner('<div />')
                            .children()
                            .slideUp(200, function () {
                                $(this).closest('tr').remove();
                            });
                    });
            });
        }
    </script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["MY_BOTS"] ?></h2>

    <div class="basicContainer">
        <a href="add_bot.php" class="button" style="margin-bottom: 20px"><?= $tr["CREATE_BOT"] ?></a>
        <table id="manageBotsTable">
            <thead>
            <tr>
                <th style="width: 150px"><?= $tr["NAME"] ?></th>
                <th style="width: 150px"><?= $tr["LAST_EDIT"] ?></th>
                <th style="width: 80px"><?= $tr["CODE_LANG"] ?></th>
                <th style="width: 120px"><?= $tr["STATE"] ?></th>
                <th style="width: 120px"><?= $tr["LEADERBOARDS"] ?></th>
                <th colspan="3"><?= $tr["OPERATIONS"] ?></th>
            </tr>
            </thead>
            <tbody>
            <?php
            $leaderboards = array();
            $res = SQL("SELECT * FROM leaderboards");
            foreach ($res as $lb) {
                $leaderboards[] = new Leaderboard($lb);
            }
            $bots = SQL("SELECT id, name, lastChangeTime, code_lang, state, compError, runError
                        FROM bots WHERE accountID = ?", $_SESSION["accountID"]);
            if ($bots == null)
                $bots = array();
            foreach ($bots as $bot) {
                $stateColumn = getBotStateText($bot["state"], $bot["id"]);
                echo '<tr data-botID="' . $bot["id"] . '" data-compiling="' . ($bot["state"] == "processing" ? "true" : "false") . '">';
                echo '<td class="botName">' . $bot["name"] . '</td>';
                echo '<td>' . $bot["lastChangeTime"] . '</td>';
                echo '<td>' . $bot["code_lang"] . '</td>';
                echo '<td class="botState">' . $stateColumn . '</td>';
                $botLeaderboards = array();
                foreach ($leaderboards as $leaderboard) {
                    if ($leaderboard->isBotParticipating($bot["id"]) && $leaderboard->isActivated())
                        $botLeaderboards[] = $leaderboard->getFirendlyName();
                }
                echo '<td>' . (count($botLeaderboards) > 0 ? implode(", ", $botLeaderboards) : " - ") . '</td>';
                echo '<td style="cursor:pointer" onclick="document.location = \'played_games.php?id=' . $bot["id"] . '\';">'
                    . '<div class="icon playedGamesIcon" title="' . $tr["PLAYED_GAMES"] . '"></div></td>';
                echo '<td style="cursor:pointer" onclick="document.location = \'edit_bot.php?id=' . $bot["id"] . '\';">
                    <div class="icon editIcon" title="' . $tr["EDIT_BOT"] . '"></div></td>';
                echo '<td style="cursor:pointer" onclick="deleteBotAsk(this)">'
                    . '<div class="icon deleteIcon" title="' . $tr["DELETE_BOT"] . '"></div></td>';
                echo '</tr>';
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