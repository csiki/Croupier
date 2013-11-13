<?php
include "php/include.php";
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$message = "";
if(isset($_GET["email"]) && isset($_GET["id"]))
{
    $res = SQL("SELECT id, activated from accounts where email = ?", $_GET["email"]);
    if(!$res)
        $message = $tr["ACCOUNT_NOT_FOUND"];
    else
    if($res[0]["activated"] == 1)
    {
        $message = $tr["ACCOUNT_ALREADY_ACTIVATED"];
    }
    else {
        $id = $res[0]["id"];
        $res = SQL("SELECT hash from account_activation where id = ?", $id);
        //if activated
        if($_GET["id"] == $res[0]["hash"])
        {
            SQL("UPDATE accounts SET activated = 1 WHERE id = ?", $id);
            SQL("DELETE FROM account_activation WHERE id = ?", $id);
            $message = $tr["ACCOUNT_ACTIVATED"];
        }
        else
            $message = $tr["ACCOUNT_HASH_ERR"];
    }
}
else
    die("Invalid request.");
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/sha512.js"></script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?php print($tr["REG_ACCOUNT_TITLE"]); ?></h2>
    <?=$message?>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>