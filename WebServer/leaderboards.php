<?php
include "php/include.php";
include "get_leaderboard.php";
needLogin();

function getBotInfos($lb)
{
    if ($result = SQL("SELECT bots.id, bots.name, bots.state,
                        (SELECT EXISTS(SELECT * FROM leaderboard".$lb." WHERE botID = bots.id)) as participated
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
            $("#tabsContainer ul li").on("click", tabClicked);

            $("div.bots table tbody, div.botsOwned table tbody").each(function () {
                if ($(this).children("tr").length == 0) {
                    addTableEmpty($(this));
                }
            });

            updateLeaderboard(1);//todo generic loading
        });

        function addTableEmpty($tbody) {//todo check empty #bots
            var $row = $('<td colspan="3" id="noBotsFound"><?=$tr["NO_BOTS_FOUND"]?></td>');
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
            var $p = $(element).parent();
            $(element).hide();
            participate_bot($("#tabsContainer ul li").index($("#tabsContainer ul li.tabs-active")) + 1, id, false,
                function () {
                    $p.children(".participate").show();
                    updateLeaderboard(1);
                });
        }

        function participate(element, id) {
            var $p = $(element).parent();
            $(element).hide();
            participate_bot($("#tabsContainer ul li").index($("#tabsContainer ul li.tabs-active")) + 1, id, true, function () {
                $p.children(".backtrack").show();
                updateLeaderboard(1);
            });
        }

        function updateLeaderboard(id) {
            var $tbody = $("#tab-" + id + " div.bots table tbody");
            $.getJSON( "get_leaderboard.php",
                { leaderBoard: id },
                function( data ) {
                    if(data.length != 0)
                        $tbody.html("");
                    $.each( data, function( i, val ) {
                        $( "<tr/>", {
                            html: "<td>" + val.name + "</td><td>"+val.username+"</td><td>"+val.score+"</td>"
                        }).appendTo($tbody);
                    });
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
            <li class="tabs-active">Leaderboards1</li>
            <li>Leaderboards2</li>
            <li>Leaderboards3</li>
        </ul>
        <div id="tabPages">
            <div id="tab-1">
                <?php
                echo '<div class="bots">
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
                $rows = getBotInfos(1);
                for ($i = 0; $i < count($rows); $i++) {
                    echo '<tr>';
                    echo '<td>' . $rows[$i]["name"] . '</td>';
                    echo '<td>' . $rows[$i]["state"] . '</td>';
                    echo '<td>';
                    echo '<a href="javascript:;" ' . ($rows[$i]["participated"] == '1' ? '' : 'style="display:none"') . 'class="button backtrack" onclick="backtrack(this, ' . $rows[$i]["id"] . ')">' . $tr["BACKTRACK"] . '</a>';
                    echo '<a class="disabledButton" ' . ($rows[$i]["participated"] != '1' && ($rows[$i]["state"] != "ok") ? '' : 'style="display:none"') . '>' . $tr["PARTICIPATE"] . '</a>';
                    echo '<a href="javascript:;" ' . ($rows[$i]["participated"] != '1' && ($rows[$i]["state"] == "ok") ? '' : 'style="display:none"') . 'class="button participate" onclick="participate(this, ' . $rows[$i]["id"] . ')">' . $tr["PARTICIPATE"] . '</a>';
                    echo '</td>';
                    echo '</tr>';
                }
                echo '</tbody>
                    </table>
                </div>';
                ?>
            </div>
            <div id="tab-2" style="display: none">
                vbvbv
            </div>
            <div id="tab-3" style="display: none">
                vbvbbvbb
            </div>
        </div>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>