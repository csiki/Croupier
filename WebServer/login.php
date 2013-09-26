<?php
include "php/include.php";
if($loggedin){
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
    <h2><?php print($tr["LOGIN"]); ?></h2>
    <form action="php/process_login.php" method="post" name="login_form">
        Email: <input type="text" name="email">
        <br />
        <br />
        Password: <input type="password" name="pass">
        <br />
        <br />
        <input type="submit" value="Login" onclick="sendForm(this.form, this.form.pass);">
    </form>
</div>
<div id="footer"></div>
</body>
</html>