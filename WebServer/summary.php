<?php
include "php/include.php";
?>
<!DOCTYPE html>
<html>
<head>
    <title><?php print($tr["WEBPAGENAME"]); ?></title>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="style/main.css">
</head>
<body>
<div id="header">
    <div id="topmenu">
    <ul>
        <li><a href="./"><?php print($tr["MAIN"]); ?></a></li>
        <?php if ($loggedin) { ?>
            <li><a href="summary.php"><?php print($tr["SUMMARY"]); ?></a></li>
            <li><a href="logout.php"><?php print($tr["LOGOUT"]); ?></a></li>
        <?php } else { ?>
            <li><a href="login.php"><?php print($tr["LOGIN"]); ?></a></li>
        <?php } ?>
    </ul>
        </div>
    <div id="lang">
        <a href="setlang.php?lang=en">EN</a> |
        <a href="setlang.php?lang=hu">HU</a>
    </div>
</div>
<div id="main">
    <h2><?php print($tr["SUMMARY"]); ?></h2>
    <?php
    if(!$loggedin) echo 'You are not authorized to access this page, please <a href="login.php">login.</a><br/>';
    else {
    ?>
    <p><?php print($tr["SUMMARY_WELCOME"]); ?></p>
    <?php } ?>
</div>
<div id="footer"></div>
</body>
</html>