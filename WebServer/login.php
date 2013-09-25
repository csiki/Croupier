<?php
include "functions.php";
include "connect_db.php";
sec_session_start();
$loggedin = false;
if(login_check($mysqli) == true)
    $loggedin = true;

if($loggedin) header('Location: summary.php');
?>
<!DOCTYPE html>
<html>
<head>
    <title>Croupier poker AI</title>
    <link rel="stylesheet" type="text/css" href="style/main.css">
    <script type="text/javascript" src="sha512.js"></script>
    <script type="text/javascript">
        function sendForm(form, password)
        {
            var p = document.createElement("input");
            form.appendChild(p);
            p.name = "p";
            p.type = "hidden"
            var shaObj = new jsSHA(password.value, "TEXT");
            p.value = shaObj.getHash("SHA-512", "HEX");
            password.value = "";
            form.submit();
        }
    </script>
</head>
<body>
<div id="topmenu">
    <ul>
        <li><a href="./">Main</a></li>
        <li><a href="login.php">Login</a></li>;
    </ul>
</div>
<div id="main">
    <h2>Login</h2>
    <form action="process_login.php" method="post" name="login_form">
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