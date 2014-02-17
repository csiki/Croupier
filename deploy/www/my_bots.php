<?php
require_once "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script>
        $(function () {
            $("#manageBotsTable tbody tr").on('remove', function () {
                if ($("#manageBotsTable tbody tr").length == 1) {
                    addTableEmpty();
                }
            });

            if ($("#manageBotsTable tbody tr").length == 0) {
                addTableEmpty();
            }
        });

        function addTableEmpty() {
            var $row = $('<td colspan="6" style="text-align:center"><?=$tr["NO_BOTS_FOUND"]?></td>');
            $row.hide();
            $row.appendTo($("#manageBotsTable tbody"));
            $row.slideDown(200);
        }

        function deleteBotAsk(element, id) {
            messageBoxYesNo("<?=$tr["DELETE_BOT_CONF"]?>", "<?=$tr["YES"]?>", "<?=$tr["NO"]?>", function () {
                addLoading(element)
                deleteBot(id);
            });
        }

        function deleteBot(id) {
            var xmlhttp = getAJAX();
            xmlhttp.onreadystatechange = function () {
                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                    if (xmlhttp.responseText == "1") {
                        $("#bot" + id).children('td')
                            .animate({ 'padding-top': 0, 'padding-bottom': 0}, 100, "linear", function () {
                                $(this)
                                    .wrapInner('<div />')
                                    .children()
                                    .slideUp(200, function () {
                                        $(this).closest('tr').remove();
                                    });
                            });
                    }
                }
            }
            xmlhttp.open("GET", "delete_bot.php?botID=" + id, true);
            xmlhttp.send();
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
                <th style="width: 80px"><?= $tr["STATE"] ?></th>
                <th colspan="3"><?= $tr["OPERATIONS"] ?></th>
            </tr>
            </thead>
            <tbody>
            <?php
            $bots = SQL("SELECT id, name, lastChangeTime, code_lang, state, compError, runError
                        FROM bots WHERE accountID = ?", $_SESSION["accountID"]);
            if ($bots == null)
                $bots = array();
            foreach($bots as $bot) {
                $state = $bot["state"];
                if($bot["state"] == 'compilation')
                {
                    $state = "<a href=\"javascript:;\" onclick=\"messageBoxOk('".htmlspecialchars($bot["compError"])."');\">".$bot["state"]."</a>";
                }
                echo '<tr id="bot' . $bot["id"] . '" >';
                echo '<td>' . $bot["name"] . '</td>';
                echo '<td>' . $bot["lastChangeTime"] . '</td>';
                echo '<td>' . $bot["code_lang"] . '</td>';
                echo '<td>' . $state . '</td>';
                echo '<td style="cursor:pointer" onclick="document.location = \'played_games.php?id=' . $bot["id"] . '\';">'
                    . '<div class="icon playedGamesIcon" title="' . $tr["PLAYED_GAMES"] . '"></div></td>';
                echo '<td style="cursor:pointer" onclick="document.location = \'edit_bot.php?id=' . $bot["id"] . '\';">
                    <div class="icon editIcon" title="' . $tr["EDIT_BOT"] . '"></div></td>';
                echo '<td style="cursor:pointer" onclick="deleteBotAsk(this, ' . $bot["id"] . ')">'
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