<?php
include "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["SUMMARY"]?></h2>
    <p><?=$tr["SUMMARY_WELCOME"]?></p>
</div>
<div id="footer"></div>
</body>
</html>