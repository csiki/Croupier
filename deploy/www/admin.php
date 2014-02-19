<?php
require_once "php/include.php";
needLogin();
if (!$admin) {
    header('Location: ./');
    exit();
}

?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <link rel="stylesheet" href="../style/admin.css">
    <link rel="stylesheet" href="../style/codemirror.css">
    <link rel="stylesheet" href="../style/codemirror-neat-modified.css">
    <script type="text/javascript" src="../scripts/jsapi_google.js"></script>
    <script type="text/javascript" src="../scripts/codemirror-compressed.js"></script>
    <script type="text/javascript" src="../scripts/css.js"></script>
    <script type="text/javascript" src="../scripts/xml.js"></script>
    <script type="text/javascript" src="../scripts/javascript.js"></script>
    <script>
        var editor;
        google.load('visualization', '1', {packages: ['corechart'], 'language': 'en'});
        window.onload =
            function () {
                $("#chartSelector").change(setChart);
                chart1();
                window.setInterval(renderPreview, 600);
                editor = CodeMirror.fromTextArea($("#content").get(0), {
                    mode: "text/html",
                    tabMode: "indent"
                });
                $("#postButton").on("click", sendPost);
            };

        function sendPost() {
            $("#postMessage").html("Posting...");
            $.post("admin_post.php",
                { title: $("#title").val(), content: editor.getValue() },
                function (data) {
                    $("#postMessage").html(data);
                },
                "json");
        }

        function renderPreview() {
            $("#postPreview").html(editor.getValue());
        }

        function setChart() {
            window[$("#chartSelector").val()]();
        }

        function chart1() {
            var data = new google.visualization.DataTable();
            data.addColumn('date', 'x');
            data.addColumn('number', 'Total number of bots added');
            <?php
                $result = SQL("SELECT * FROM stat_bots_added");
                $sum = 0;
                for($i=0; $i<count($result); $i++)
                {
                    $sum += intval($result[$i]["count"]);
                    echo "data.addRow([new Date('" .$result[$i]["date"]. "'), ".$sum."]);\n";
                }
            ?>
            drawLinechart(data);
        }

        function chart2() {
            var data = new google.visualization.DataTable();
            data.addColumn('date', 'x');
            data.addColumn('number', 'Total number of registered accounts');
            <?php
                $result = SQL("SELECT * FROM stat_accounts_added");
                $sum = 0;
                for($i=0; $i<count($result); $i++)
                {
                    $sum += intval($result[$i]["count"]);
                    echo "data.addRow([new Date('" .$result[$i]["date"]. "'), ".$sum."]);\n";
                }
            ?>
            drawLinechart(data);
        }

        function chart3() {
            var data = new google.visualization.DataTable();
            data.addColumn('date', 'x');
            data.addColumn('number', 'Number of page loads');
            <?php
                $result = SQL("SELECT * FROM stat_pageload");
                for($i=0; $i <count($result); $i++)
                {
                    echo "data.addRow([new Date('" .$result[$i]["date"]. "'), ".$result[$i]["count"]."]);\n";
                }
            ?>
            drawLinechart(data);
        }

        function drawLinechart(data) {
            new google.visualization.LineChart(document.getElementById('visualization')).
                draw(data, {curveType: "none",
                    width: 700,
                    height: 400,
                    pointSize: 8,
                    hAxis: {format: 'MMM dd'}
                }
            );
        }
    </script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h1>Admin</h1>

    <h2>New post</h2>

    <div id="post">
        <label for="title">Title</label><br/>
        <input name="title" id="title" type="text" value="" autofocus>
        <br/>
        <label for="content">Content (html)</label><br/>
        <textarea cols="80" rows="20" name="content" id="content"></textarea>
        <br/><br/>
        <input type="submit" class="button" value="Post" id="postButton">
        <span id="postMessage"></span>
    </div>
    <div id="postPreview"></div>

    <br/>

    <h2>Charts</h2>
    <select id="chartSelector">
        <option value="chart1">Added bots</option>
        <option value="chart2">Registered users</option>
        <option value="chart3">Pageloads</option>
    </select>
    <br/>
    <br/>

    <div id="visualization" style="width: 500px; height: 400px;"></div>

    <h2>Accounts</h2>

    <table>
        <thead>
        <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Email</th>
            <th>IsAdmin</th>
            <th>Last Online</th>
            <th>Language</th>
            <th>BotCount</th>
        </tr>
        </thead>
        <tbody>
        <?php
        $botsRes = SQL("SELECT accountID FROM bots");
        $bots = array();
        for ($i = 0; $i < count($botsRes); $i++) {
            if (!isset($bots[$botsRes[$i]["accountID"]]))
                $bots[$botsRes[$i]["accountID"]] = 0;
            $bots[$botsRes[$i]["accountID"]]++;
        }
        $acc = SQL("SELECT id, username, email, admin, lastOnline, lang FROM accounts");
        for ($i = 0; $i < count($acc); $i++) {
            echo '<tr>';
            echo '<td>' . $acc[$i]["id"] . '</td>';
            echo '<td>' . $acc[$i]["username"] . '</td>';
            echo '<td>' . $acc[$i]["email"] . '</td>';
            echo '<td>' . $acc[$i]["admin"] . '</td>';
            echo '<td>' . $acc[$i]["lastOnline"] . '</td>';
            echo '<td>' . $acc[$i]["lang"] . '</td>';
            echo '<td>' . (isset($bots[$acc[$i]["id"]]) ? $bots[$acc[$i]["id"]] : "0") . '</td>';
            echo '</tr>';
        }
        ?>
        </tbody>
    </table>
    <br/>

    <h2>Bots</h2>

    <table>
        <thead>
        <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Owner</th>
            <th>Last Changed</th>
            <th>CodeLang</th>
            <th>State</th>
            <th colspan="2"><?= $tr["OPERATIONS"] ?></th>
        </tr>
        </thead>
        <tbody>
        <?php
        $accRes = SQL("SELECT id, username FROM accounts");
        $acc = array();
        for ($i = 0; $i < count($accRes); $i++) {
            $acc[$accRes[$i]["id"]] = $accRes[$i]["username"];
        }
        $bots = SQL("SELECT id, accountID, name, lastChangeTime, code_lang, state FROM bots");
        for ($i = 0; $i < count($bots); $i++) {
            $stateColumn = "";
            switch($bots[$i]["state"])
            {
                case 'ok':
                    $stateColumn = $tr["STATE_OK"];
                    break;
                case 'processing':
                    $stateColumn = $tr["STATE_PROCESSING"];
                    break;
                case 'compilation':
                    $stateColumn = "<a href=\"show_compileError.php?botID=" . $bots[$i]["id"] . "\">".$tr["STATE_COMPILATION"]."</a>";
                    break;
                case 'runtime':
                    $stateColumn = $tr["STATE_RUNTIME"];
                    break;
            }
            echo '<tr>';
            echo '<td>' . $bots[$i]["id"] . '</td>';
            echo '<td>' . $bots[$i]["name"] . '</td>';
            echo '<td>' . $acc[$bots[$i]["accountID"]] . '</td>';
            echo '<td>' . $bots[$i]["lastChangeTime"] . '</td>';
            echo '<td>' . $bots[$i]["code_lang"] . '</td>';
            echo '<td>' . $stateColumn . '</td>';
            echo '<td style="cursor:pointer" onclick="document.location = \'played_games.php?id=' . $bots[$i]["id"] . '\';">'
                . '<div class="icon playedGamesIcon" title="' . $tr["PLAYED_GAMES"] . '"></div></td>';
            echo '<td style="cursor:pointer" onclick="document.location = \'show_bot.php?id=' . $bots[$i]["id"] . '\';">
                    <div class="icon editIcon" title="Show bot"></div></td>';
            echo '</tr>';
        }
        ?>
        </tbody>
    </table>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>