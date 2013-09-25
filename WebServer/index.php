<?php
include "functions.php";
include "connect_db.php";
sec_session_start();
$loggedin = false;
if(login_check($mysqli) == true)
    $loggedin = true;
?>
<!DOCTYPE html>
<html>
<head>
    <title>Croupier poker AI</title>
    <link rel="stylesheet" type="text/css" href="style/main.css">
</head>
<body>
<div id="topmenu">
     <ul>
         <li><a href="./">Main</a></li>
         <?php if($loggedin){ ?>
             <li><a href="summary.php">Summary</a></li>
             <li><a href="logout.php">Logout</a></li>
         <?php } else {
         echo '<li><a href="login.php">Login</a></li>';
         } ?>
     </ul>
</div>
<div id="main">
    <h2>Croupier poker AI</h2>
</div>
<div id="footer"></div>
</body>
</html>