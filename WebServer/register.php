<?php
include "php/include.php";
if($loggedin)
{
    header('Location: summary.php');
    exit();
}
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
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?php print($tr["REGISTER"]); ?></h2>
    <?php
    if(isset($_GET["success"]) && $_GET["success"] == 1)
        echo $tr["REG_THANKS"];
        else
    echo '<form action="php/process_register.php" method="post" name="login_form">
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
    </form>';
    ?>
</div>
<div id="footer"></div>
</body>
</html>