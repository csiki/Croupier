<?php
include "php/include.php";
needLogin();
if (!$admin) {
    header('Location: ./');
    exit();
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <link rel="stylesheet" type="text/css" href="style/admin.css">
    <script type="text/javascript" src="https://www.google.com/jsapi"></script>
    <script>
        google.load('visualization', '1', {packages: ['corechart'], 'language': 'en'});
        window.onload =
            function () {
                $("#chartSelector").change(setChart);
                chart1();
            };

        function setChart() {
            window[$("#chartSelector").val()]();
        }

        function chart1() {
            var data = new google.visualization.DataTable();
            data.addColumn('date', 'x');
            data.addColumn('number', 'Number of bots added');
            <?php
                $result = SQL("SELECT * FROM stat_bots_added");
                $sum = 0;
                for($i=0; $i<count($result); $i++)
                {
                    $sum += intval($result[$i]["count"]);
                    echo "data.addRow([new Date('" .$result[$i]["time"]. "'), ".$sum."]);\n";
                }
            ?>
            drawLinechart(data);
        }
        function chart2() {
            var data = new google.visualization.DataTable();
            data.addColumn('date', 'x');
            data.addColumn('number', 'Number of registered accounts');
            <?php
                $result = SQL("SELECT * FROM stat_accounts_added");
                $sum = 0;
                for($i=0; $i<count($result); $i++)
                {
                    $sum += intval($result[$i]["count"]);
                    echo "data.addRow([new Date('" .$result[$i]["time"]. "'), ".$sum."]);\n";
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
<?php include "php/header.php"; ?>
<div id="main">
    <h1>Admin</h1>

    <h2>Charts</h2>

    <p>
        <select id="chartSelector">
            <option value="chart1">Added bots</option>
            <option value="chart2">Registered users</option>
        </select>

    <div id="visualization" style="width: 500px; height: 400px;"></div>
    </p>
    <h2>Accounts</h2>

    <p>
    <table>
        <thead>
        <th>ID</th>
        <th>Name</th>
        <th>Email</th>
        <th>IsAdmin</th>
        <th>Last Online</th>
        <th>Language</th>
        <th>BotCount</th>
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
        $acc = SQL("SELECT id, username, email, admin, lastOnline, language FROM accounts");
        for ($i = 0; $i < count($acc); $i++) {
            echo '<tr>';
            echo '<td>' . $acc[$i]["id"] . '</td>';
            echo '<td>' . $acc[$i]["username"] . '</td>';
            echo '<td>' . $acc[$i]["email"] . '</td>';
            echo '<td>' . $acc[$i]["admin"] . '</td>';
            echo '<td>' . $acc[$i]["lastOnline"] . '</td>';
            echo '<td>' . $acc[$i]["language"] . '</td>';
            echo '<td>' . (isset($bots[$acc[$i]["id"]]) ? $bots[$acc[$i]["id"]] : "0") . '</td>';
            echo '</tr>';
        }
        ?>
        </tbody>
    </table>
    </p>
    <h2>Bots</h2>

    <p>
    <table>
        <thead>
        <th>ID</th>
        <th>Name</th>
        <th>Owner</th>
        <th>Last Changed</th>
        <th>CodeLang</th>
        <th>State</th>
        <th>Participate</th>
        </thead>
        <tbody>
        <?php
        $accRes = SQL("SELECT id, username FROM accounts");
        $acc = array();
        for ($i = 0; $i < count($accRes); $i++) {
            $acc[$accRes[$i]["id"]] = $accRes[$i]["username"];
        }
        $bots = SQL("SELECT id, accountID, name, lastChangeTime, code_lang, state, participate FROM bots");
        for ($i = 0; $i < count($bots); $i++) {
            echo '<tr>';
            echo '<td>' . $bots[$i]["id"] . '</td>';
            echo '<td>' . $bots[$i]["name"] . '</td>';
            echo '<td>' . $acc[$bots[$i]["accountID"]] . '</td>';
            echo '<td>' . $bots[$i]["lastChangeTime"] . '</td>';
            echo '<td>' . $bots[$i]["code_lang"] . '</td>';
            echo '<td>' . $bots[$i]["state"] . '</td>';
            echo '<td>' . $bots[$i]["participate"] . '</td>';
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