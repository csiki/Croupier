<?php
include "php/include.php";
needLogin();
function getBotInfos($mysqli)
{
    if ($result = SQL("SELECT id, name, lastChangeTime, code_lang, state, participate
    FROM bots WHERE accountID = ?", $_SESSION["accountID"])
    ) {
        if ($result != null)
            return $result;
    }
    return array();
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/main.js"></script>
    <script>
        var checkedCount = 0;
        var msgPARTICIPATE_ADD = '<?=$tr["PARTICIPATE_ADD"]?>';
        var msgPARTICIPATE_REMOVE = '<?=$tr["PARTICIPATE_REMOVE"]?>';
        var msgPARTICIPATE_ADDED = '<?=$tr["PARTICIPATE_ADDED"]?>';
        var msgPARTICIPATE_REMOVED = '<?=$tr["PARTICIPATE_REMOVED"]?>';
        var msgPARTICIPATE_ADDALL = '<?=$tr["PARTICIPATE_ADDALL"]?>';
        var msgPARTICIPATE_REMOVEALL = '<?=$tr["PARTICIPATE_REMOVEALL"]?>';
        window.onload = function () {
            $(".check").each(function () {
                if ($(this).prop("checked")) checkedCount++;
            });
            checkCheck();
            $(".check").on("click", function () {
                var checked = $(this).prop('checked');
                if (checked)
                    checkedCount++;
                else
                    checkedCount--;
                checkCheck();
                if (messageBoxAsk(checked ? msgPARTICIPATE_ADD : msgPARTICIPATE_REMOVE))
                    participate_bot($(this).prop("name").substr(3), checked,
                        function () {
                            messageBox(checked ? msgPARTICIPATE_ADDED : msgPARTICIPATE_REMOVED);
                        });
                else
                    $(this).prop("checked", !checked);
            });
            $("#checkAll").on("click", function () {
                var checked = $(this).prop('checked');
                if (messageBoxAsk(checked ? msgPARTICIPATE_ADDALL : msgPARTICIPATE_REMOVEALL)) {
                    $('.check').prop('checked', checked);
                    $('.check').each(function () {
                        participate_bot($(this).prop("name").substr(3), checked,
                            function () {
                                messageBox(checked ? msgPARTICIPATE_ADDED : msgPARTICIPATE_REMOVED);
                            });
                    });
                }
                else
                    $(this).prop("checked", !checked);
            });
        };
        function checkCheck() {
            if (checkedCount == 0)
                $("#checkAll").prop('checked', false);
            else
                $("#checkAll").prop('checked', true);
        }
        function deleteBotAsk(id) {
            if (messageBoxAsk('<?=$tr["DELETE_BOT_CONF"]?>'))
                deleteBot(id);
        }
    </script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["MANAGE_BOTS"]?></h2>
    <a href="add_bot.php" class="button"><?=$tr["ADDBOT"]?></a>

    <p>
    <table id="manageBotsTable">
        <thead>
        <tr>
            <th style="width: 30px"><input type="checkbox" name="all" id="checkAll"></th>
            <th style="width: 150px"><?=$tr["NAME"]?></th>
            <th style="width: 150px"><?=$tr["DATE_TIME"]?></th>
            <th style="width: 80px"><?=$tr["CODE_LANG"]?></th>
            <th style="width: 80px"><?=$tr["STATE"]?></th>
            <th style="width: 30px"></th>
        </tr>
        </thead>
        <tbody>
        <?php
        $rows = getBotInfos($mysqli);
        if (count($rows) == 0) {
            echo '<td colspan="6" id="noBotsFound">' . $tr["NO_BOTS_FOUND"] . '</td>';
        } else
            for ($i = 0; $i < count($rows); $i++) {
                $a = 'onclick="document.location = \'edit_bot.php?botid=' . $rows[$i]["id"] . '\';"';
                echo '<tr id="bot' . $rows[$i]["id"] . '" >';
                echo '<td style="width: 30px"><input type="checkbox" class="check" name="chk' . $rows[$i]["id"] . '"
                ' . ($rows[$i]["participate"] ? 'checked' : '') . '></td>';
                echo '<td ' . $a . '>' . $rows[$i]["name"] . '</td>';
                echo '<td ' . $a . '>' . $rows[$i]["lastChangeTime"] . '</td>';
                echo '<td ' . $a . '>' . $rows[$i]["code_lang"] . '</td>';
                echo '<td ' . $a . '>' . $rows[$i]["state"] . '</td>';
                echo '<td><a onclick="deleteBotAsk(' . $rows[$i]["id"] . ')">del</a></td>';
                echo '</tr>';
            }
        ?>
        </tbody>
    </table>
    </p>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>