<?php
include "php/include.php";
include "get_leaderboard.php";
needLogin();

function getBotInfos($lb)
{
    if ($result = SQL("SELECT bots.id, bots.name, bots.state,
                        (SELECT EXISTS(SELECT * FROM leaderboard" . $lb . " WHERE botID = bots.id)) as participated
                        FROM bots
                        WHERE bots.accountID = ?", $_SESSION["accountID"])
    ) {
        if ($result != null)
            return $result;
    }
    return array();
}

$leaderboards = SQL("SELECT id from leaderboards");

?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script>
        $(function () {
            $("#tabsContainer ul li").on("click", tabClicked);

            $("#tabPages div div table tbody").each(function () {
                if ($(this).children("tr").length == 0) {
                    setTableLoading($(this));
                }
            });

            $("[id^='tab-']").each(function () {
                updateLeaderboard($(this));
            });
        });

        function setTableEmpty($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="'+$columns+'" id="noBotsFound"><?=$tr["NO_BOTS_FOUND"]?></td>');
            $row.appendTo($($tbody));
        }

        function setTableLoading($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="'+$columns+'" id="noBotsFound"><?=$tr["LOADING"]?></td>');
            $row.appendTo($($tbody));
        }

        function tabClicked() {
            if ($(this).hasClass("tabs-active"))
                return;
            $("#tabsContainer ul li").removeClass("tabs-active");
            $(this).addClass("tabs-active");
            $("#tabPages>div").hide();
            $("#tab-" + ($("#tabsContainer ul li").index($(this)) + 1)).show();
        }

        function participate_bot(lb, id, isAdd, complete) {
            var xmlhttp = getAJAX();
            xmlhttp.onreadystatechange = function () {
                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                    complete.call();
                }
            }
            xmlhttp.open("GET", "participate_bot.php?leaderBoard=" + lb + "&botID=" + id + "&action=" + (isAdd ? "1" : "0"), true);
            xmlhttp.send();
        }

        function backtrack(element, id) {
            var $tab = $(element).parents("[id^='tab-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loading").show();
            participate_bot($("#tabsContainer ul li").index($("#tabsContainer ul li.tabs-active")) + 1, id, false,
                function () {
                    $ptd.children(".participate").show();
                    $ptd.children(".loading").hide();
                    updateLeaderboard($tab);
                });
        }

        function participate(element, id) {
            var $tab = $(element).parents("[id^='tab-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loading").show();
            participate_bot($("#tabsContainer ul li").index($("#tabsContainer ul li.tabs-active")) + 1, id, true, function () {
                $ptd.children(".backtrack").show();
                $ptd.children(".loading").hide();
                updateLeaderboard($tab);
            });
        }

        function updateLeaderboard(tab) {
            var $tbody = $(tab).find(".leaderboard table tbody");
            setTableLoading($tbody);
            $.getJSON("get_leaderboard.php",
                { leaderBoard: $(tab).attr('id').substr(4) },
                function (data) {
                    if (data.length != 0) {
                        $tbody.html("");
                        $.each(data, function (i, val) {
                            $("<tr/>", {
                                html: "<td>" + val.name + "</td><td>" + val.username + "</td><td>" + val.score + "</td>"
                            }).appendTo($tbody);
                        });
                    }
                    else
                        setTableEmpty($tbody);
                });
        }
    </script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["LEADERBOARDS"]?></h2>

    <div id="tabsContainer">
        <ul>
            <?php
            for ($i = 0; $i < count($leaderboards); $i++) {
                echo '<li ' . ($i == 0 ? 'class="tabs-active"' : '') . '>Leaderboard' . $leaderboards[$i]["id"] . '</li>';
            }
            ?>
        </ul>
        <div id="tabPages">
            <?php
            for ($i = 0; $i < count($leaderboards); $i++) {
                echo '<div id="tab-' . ($i + 1) . '" ' . ($i == 0 ? '' : 'style="display:none;"') . '>';
                echo '<div class="leaderboard">
                <table>
                    <thead>
                    <tr>
                        <th style="width: 150px">' . $tr["NAME"] . '</th>
                        <th style="width: 150px">' . $tr["USERNAME"] . '</th>
                        <th style="width: 80px">' . $tr["SCORE"] . '</th>
                    </tr>
                    </thead>
                    <tbody>';
                echo '</tbody></table>
                </div>
                <div class="botsOwned">
                    <table>
                        <thead>
                        <tr>
                            <th style="width: 150px">' . $tr["NAME"] . '</th>
                            <th style="width: 150px">' . $tr["STATE"] . '</th>
                            <th style="width: 30px" colspan="2">' . $tr["OPERATIONS"] . '</th>
                        </tr>
                        </thead>
                        <tbody>';
                $rows = getBotInfos($leaderboards[$i]["id"]);
                for ($j = 0; $j < count($rows); $j++) {
                    echo '<tr>';
                    echo '<td>' . $rows[$j]["name"] . '</td>';
                    echo '<td>' . $rows[$j]["state"] . '</td>';
                    echo '<td>';
                    echo '<a href="javascript:;" ' . ($rows[$j]["participated"] == '1' ? '' : 'style="display:none"') . 'class="button backtrack" onclick="backtrack(this, ' . $rows[$j]["id"] . ')">' . $tr["BACKTRACK"] . '</a>';
                    echo '<a class="button disabledButton" ' . ($rows[$j]["participated"] != '1' && ($rows[$j]["state"] != "ok") ? '' : 'style="display:none"') . '>' . $tr["PARTICIPATE"] . '</a>';
                    echo '<a class="button disabledButton loading" style="display:none">' . $tr["LOADING"] . '</a>';
                    echo '<a href="javascript:;" ' . ($rows[$j]["participated"] != '1' && ($rows[$j]["state"] == "ok") ? '' : 'style="display:none"') . 'class="button participate" onclick="participate(this, ' . $rows[$j]["id"] . ')">' . $tr["PARTICIPATE"] . '</a>';
                    echo '</td>';
                    echo '</tr>';
                }
                echo '</tbody>
                    </table>
                </div>';
                echo '</div>';
            }
            ?>
        </div>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>