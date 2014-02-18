<?php
require_once "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["HELP"] ?></h2>
    <?php
    echo sprintf($tr["HELP_MAIN"], "docs/mi_dev/miDev.html");
    ?>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>