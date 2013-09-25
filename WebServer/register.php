<?php
include "php/include.php";
if($loggedin) header('Location: summary.php');
?>
<!DOCTYPE html>
<html>
<head>
    <title><?php print($tr["WEBPAGENAME"]); ?></title>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="style/main.css">
    <script type="text/javascript" src="scripts/sha512.js"></script>
    <script type="text/javascript" src="scripts/main.js"></script>
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
    <h2>Register</h2>
    <form action="php/process_register.php" method="post" name="login_form">
        Name: <input type="text" name="name">
        <br />
        <br />
        Email: <input type="text" name="email">
        <br />
        <br />
        Password: <input type="password" name="pass">
        <br />
        <br />
        <input type="submit" value="Register" onclick="sendForm(this.form, this.form.pass);">
    </form>
</div>
<div id="footer"></div>
</body>
</html>