<?php
include "php/include.php";
needLogin();
function getBotInfos($mysqli)
{
    if ($stmt = $mysqli->prepare("SELECT id, name, add_time, code_lang, state
    FROM bots WHERE account_id = ?")
    ) {
        $stmt->bind_param('i', $_SESSION["account_id"]);
        $stmt->execute();
        $result = $stmt->get_result();
        $arr = $result->fetch_all(MYSQLI_ASSOC);
        $stmt->close();
        return $arr;
    }
    return array();
}
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["MANAGE_BOTS"]?></h2>
    <a href="add_bot.php" class="button"><?=$tr["ADDBOT"]?></a>
    <p>
    <table id="manageBotsTable">
        <thead>
        <tr>
            <td style="width: 50px">#</td>
            <td style="width: 100px"><?=$tr["NAME"]?></td>
            <td style="width: 150px"><?=$tr["DATE_TIME"]?></td>
            <td style="width: 80px"><?=$tr["CODE_LANG"]?></td>
            <td style="width: 80px"><?=$tr["STATE"]?></td>
        </tr>
        </thead>
        <tbody>
        <?php
        $rows = getBotInfos($mysqli);
        for ($i = 0; $i < count($rows); $i++) {
            echo '<a href="WUT"><tr onclick="document.location = \'show_bot.php?botid='.$rows[$i]["id"].'\';">';
            echo '<td>' . $rows[$i]["id"] . '</td>';
            echo '<td>' . $rows[$i]["name"] . '</td>';
            echo '<td>' . $rows[$i]["add_time"] . '</td>';
            echo '<td>' . $rows[$i]["code_lang"] . '</td>';
            echo '<td>' . $rows[$i]["state"] . '</td>';
            echo '</tr></a>';
        }
        ?>
        </tbody>
    </table>
    </p>
</div>
<div id="footer"></div>
</body>
</html>