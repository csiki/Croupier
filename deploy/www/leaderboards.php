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
            $(".leaderboardsMenuElement").on("click", menuElementClicked);
            $(".backToMenu a").on("click", backToMenuClicked);

            $("[id^='lb-']").each(function () {
                updateLeaderboard($(this));
            });
        });

        function setTableEmpty($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="' + $columns + '" style="text-align:center"><?=$tr["NO_BOTS_FOUND"]?></td>');
            $row.appendTo($($tbody));
        }

        function setTableLoading($tbody) {
            var $columns = $tbody.parent().find("thead th").length;
            $tbody.html("");
            var $row = $('<td colspan="' + $columns + '" style="text-align:center"><?=$tr["LOADING"]?></td>');
            $row.appendTo($($tbody));
        }

        function menuElementClicked() {
            $("#leaderboardsMenu").fadeOut(300);
            //$("#leaderboardContainer").fadeIn(300);
            $("#lb-" + ($("#leaderboardsMenu div").index($(this)) + 1)).delay(290).fadeIn(300);
        }

        function backToMenuClicked() {
            $("[id^='lb-']").fadeOut(300);
            //$("#leaderboardContainer").fadeOut(300);
            $("#leaderboardsMenu").delay(290).fadeIn(300);
        }

        function participate_bot(lb, id, isAdd, complete) {
            var xmlhttp = getAJAX(); //TODO rewrite this to JQuery
            xmlhttp.onreadystatechange = function () {
                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                    complete.call();
                }
            }
            xmlhttp.open("GET", "participate_bot.php?leaderboard=" + lb + "&botID=" + id + "&action=" + (isAdd ? "1" : "0"), true);
            xmlhttp.send();
        }

        function backtrack(element, id) {
            var $lb = $(element).parents("[id^='lb-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loadingIcon").show();
            participate_bot($lb.attr('id').substr(3), id, false,
                function () {
                    $ptd.children(".participate").show();
                    $ptd.children(".loadingIcon").hide();
                    updateLeaderboard($lb);
                });
        }

        function participate(element, id) {
            var $lb = $(element).parents("[id^='lb-']");
            var $ptd = $(element).parent();
            $(element).hide();
            $ptd.children(".loadingIcon").show();
            participate_bot($lb.attr('id').substr(3), id, true, function () {
                $ptd.children(".backtrack").show();
                $ptd.children(".loadingIcon").hide();
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
    </script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["LEADERBOARDS"] ?></h2>

    <div id="leaderboardsMenu">
        <?php
        $leaderboards = SQL("SELECT tableName from leaderboards");
        foreach ($leaderboards as $leaderboard) {
            echo '<div class="leaderboardsMenuElement">';
            echo '<span>' . $leaderboard["tableName"] . '</span>';
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
    <?php
    for ($i = 0; $i < count($leaderboards); $i++) {
        echo '<div id="lb-' . ($i + 1) . '">';
        echo '<div class="backToMenu"><a class="button">' . $tr["BACK"] . '</a></div>';
        echo '<h2 class="leaderboardTitle">' . $leaderboards[$i]["tableName"] . '</h2>';
        echo '<div class="basicContainer">';
        echo '<div class="leaderboard">
                <h3>' . $tr["PARTICIPATED_BOTS"] . '</h3>
                <table>
                    <thead>
                    <tr>
                        <th style="width: 150px">' . $tr["NAME"] . '</th>
                        <th style="width: 150px">' . $tr["OWNER"] . '</th>
                        <th style="width: 80px">' . $tr["SCORE"] . '</th>
                    </tr>
                    </thead>
                    <tbody>';
        echo '</tbody></table>
                </div>
                <div class="botsOwned">
                    <h3>' . $tr["MY_BOTS"] . '</h3>
                    <table>
                        <thead>
                        <tr>
                            <th style="width: 150px">' . $tr["NAME"] . '</th>
                            <th style="width: 150px">' . $tr["STATE"] . '</th>
                            <th style="width: 30px" colspan="2">' . $tr["OPERATIONS"] . '</th>
                        </tr>
                        </thead>
                        <tbody>';
        $bots = SQL("SELECT bots.id, bots.name, bots.state,
                        (SELECT EXISTS(SELECT * FROM " . $leaderboards[$i]["tableName"] . " WHERE botID = bots.id)) as participated
                        FROM bots
                        WHERE bots.accountID = ?", $_SESSION["accountID"]);
        if ($bots == null)
            $bots = array();
        foreach ($bots as $bot) {
            echo '<tr data-valid="' . ($bot["state"] == "ok")
                . '" data-participated="' . ($bot["participated"]) . '">';
            echo '<td>' . $bot["name"] . '</td>';
            echo '<td>' . $bot["state"] . '</td>';
            echo '<td style="text-align: center">';
            echo '<a class="button backtrack"' .
                ($bot["participated"] == '1' ? '' : 'style="display:none"') .
                ' onclick="backtrack(this, ' . $bot["id"] . ')">' . $tr["BACKTRACK"] .
                '</a>';
            echo '<a class="button disabledButton" ' .
                ($bot["participated"] != '1' && ($bot["state"] != "ok") ? '' : 'style="display:none"') . '>' .
                $tr["PARTICIPATE"] .
                '</a>';
            echo '<div class="icon loadingIcon" style="display: none"></div>';
            echo '<a class="button participate"' .
                ($bot["participated"] != '1' && ($bot["state"] == "ok") ? '' : ' style="display:none"') .
                ' onclick="participate(this, ' . $bot["id"] . ')">' .
                $tr["PARTICIPATE"] .
                '</a>';
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
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>