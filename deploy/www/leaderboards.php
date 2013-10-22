<?php
include "php/include.php";
include "get_leaderboard.php";
needLogin();

function getBotInfos($lb)
{
    if ($result = SQL("SELECT bots.id, bots.name, bots.state,
                        (SELECT EXISTS(SELECT * FROM " . $lb . " WHERE botID = bots.id)) as participated
                        FROM bots
                        WHERE bots.accountID = ?", $_SESSION["accountID"])
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
    <script>
        $(function () {
            $(".leaderboardsMenuElement").on("click", menuElementClicked);
            $(".backToMenu a").on("click", backToMenuClicked);

            $("[id^='lb-']").each(function () {
                updateLeaderboard($(this));
            });
            window.setInterval(updateBots, 600);
        });

        function setTableEmpty($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="' + $columns + '" id="noBotsFound"><?=$tr["NO_BOTS_FOUND"]?></td>');
            $row.appendTo($($tbody));
        }

        function setTableLoading($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="' + $columns + '" id="noBotsFound"><?=$tr["LOADING"]?></td>');
            $row.appendTo($($tbody));
        }

        function menuElementClicked() {
            $("#leaderboardsMenu").fadeOut(300);
            $("#leaderboardContainer").fadeIn(300);
            $("#lb-" + ($("#leaderboardsMenu div").index($(this)) + 1)).delay(290).fadeIn(300);
        }

        function backToMenuClicked() {
            $("[id^='lb-']").fadeOut(300);
            $("#leaderboardContainer").fadeOut(300);
            $("#leaderboardsMenu").delay(290).fadeIn(300);
        }

        function participate_bot(lb, id, isAdd, complete) {
            var xmlhttp = getAJAX(); //TODO rewrite this to JQuery
            xmlhttp.onreadystatechange = function () {
                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                    complete.call();
                }
            }
            xmlhttp.open("GET", "participate_bot.php?leaderBoard=" + lb + "&botID=" + id + "&action=" + (isAdd ? "1" : "0"), true);
            xmlhttp.send();
        }

        function backtrack(element, id) {
            var $lb = $(element).parents("[id^='lb-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loading").show();
            participate_bot($lb.attr('id').substr(3), id, false,
                function () {
                    $ptd.children(".participate").show();
                    $ptd.children(".loading").hide();
                    updateLeaderboard($lb);
                });
        }

        function participate(element, id) {
            var $lb = $(element).parents("[id^='lb-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loading").show();
            participate_bot($lb.attr('id').substr(3), id, true, function () {
                $ptd.children(".backtrack").show();
                $ptd.children(".loading").hide();
                updateLeaderboard($lb);
            });
        }

        function updateLeaderboard(tab) {
            var $tbody = $(tab).find(".leaderboard table tbody");
            setTableLoading($tbody);
            $.getJSON("get_leaderboard.php",
                { leaderBoard: $(tab).attr('id').substr(3) },
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

        function updateBots() {
            var $tbody = "[id^='lb-'] .leaderboard table tbody";
            setTableLoading($tbody);
            $.getJSON("get_leaderboard.php",
                { leaderBoard: $(tab).attr('id').substr(3) },
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
        <div id="leaderboardsMenu">
            <?php
            $leaderboards = SQL("SELECT tableName from leaderboards");
            for ($i = 0; $i < count($leaderboards); $i++) {
                echo '<div class="leaderboardsMenuElement">';
                echo '<span>' . $leaderboards[$i]["tableName"] . '</span>';
                echo '<ul>';
                echo '<li>
                    Itt
                    </li>
                    <li>
                    lesznek a
                    </li>
                    <li>
                    szabályok
                    </li>';
                echo '</ul>';
                echo '</div>';
            }
            ?>

        </div>
        <div id="leaderboardContainer">
            <?php
            for ($i = 0; $i < count($leaderboards); $i++) {
                echo '<div id="lb-' . ($i + 1) . '">';
                echo '<div class="backToMenu"><a class="button">' . $tr["BACK"] . '</a></div>';
                echo '<h2 class="leaderboardTitle">'. $leaderboards[$i]["tableName"] .'</h2>';
                echo '<div class="basicContainer">';
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
                $rows = getBotInfos($leaderboards[$i]["tableName"]);
                for ($j = 0; $j < count($rows); $j++) {
                    echo '<tr>';
                    echo '<td>' . $rows[$j]["name"] . '</td>';
                    echo '<td>' . $rows[$j]["state"] . '</td>';
                    echo '<td>';
                    echo '<a href="javascript:;" ' . ($rows[$j]["participated"] == '1' ? '' : 'style="display:none"') . ' class="button backtrack" onclick="backtrack(this, ' . $rows[$j]["id"] . ')">' . $tr["BACKTRACK"] . '</a>';
                    echo '<a class="button disabledButton" ' . ($rows[$j]["participated"] != '1' && ($rows[$j]["state"] != "ok") ? '' : 'style="display:none"') . '>' . $tr["PARTICIPATE"] . '</a>';
                    echo '<a class="button disabledButton loading" style="display:none">' . $tr["LOADING"] . '</a>';
                    echo '<a href="javascript:;" ' . ($rows[$j]["participated"] != '1' && ($rows[$j]["state"] == "ok") ? '' : 'style="display:none"') . 'class="button participate" onclick="participate(this, ' . $rows[$j]["id"] . ')">' . $tr["PARTICIPATE"] . '</a>';
                    echo '</td>';
                    echo '</tr>';
                }
                echo '</tbody>
                    </table>
                </div>';
                echo '</div></div>';
            }
            ?>
        </div>
    </div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>