<?php
include "php/include.php";
include "php/getbots.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["MANAGE_BOTS"]?></h2>
    <a href="add_bot.php"><?= $tr["ADDBOT"] ?></a>

    <p>
    <table id="manageBotsTable" cellspacing="0" cellpadding="0">
        <thead id="manageBotsTableHead">
        <tr>
            <td style="width: 50px">#</td>
            <td style="width: 100px"><?=$tr["NAME"]?></td>
            <td style="width: 100px"><?=$tr["DATE_TIME"]?></td>
            <td style="width: 100px"><?=$tr["CODE_LANG"]?></td>
            <td style="width: 100px"><?=$tr["STATE"]?></td>
        </tr>
        </thead>
        <tbody id="manageBotsTableBody">
        <?php
            $rows = getBotInfos($mysqli);
            for($i = 0; $i < count($rows); $i++)
            {
                $rowColor = ($i % 2) == 0 ? 'lightblue' : 'lightgrey';
                echo '<tr style="background-color: '. $rowColor.';">';
                    echo '<td>'.$rows[$i]["id"].'</td>';
                    echo '<td>'.$rows[$i]["name"].'</td>';
                    echo '<td>'.$rows[$i]["add_time"].'</td>';
                    echo '<td>'.$rows[$i]["code_lang"].'</td>';
                    echo '<td>'.$rows[$i]["state"].'</td>';
                echo '</tr>';
            }
        ?>
        </tbody>
    </table>
    </p>
</div>
<div id="footer"></div>
</body>
</html>