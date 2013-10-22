<?php
include "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["SUMMARY"]?></h2>

    <p><?=$tr["SUMMARY_WELCOME"]?><br/>TODO:statok a sima usereknek</p>

</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>