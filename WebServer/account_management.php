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
    <h2><?=$tr["ACCOUNT_MANAGEMENT"]?> - <?=$_SESSION["username"]?></h2>

</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>