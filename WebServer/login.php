<?php
include "php/include.php";
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$error = "";
$email = "";
if (isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pSize'])) {
    //form validation
    //TODO:remove comment
    /*if (!isset($_POST['email']) || !sanityCheck($_POST['email'], 'string', 7, 50) || !checkEmail($_POST['email']))
        $error = $tr["ERR_LOGIN"];
    else
        $email = $_POST['email'];

    if (!isset($_POST['pSize']) || !sanityCheck($_POST['pSize'], 'numeric', 0, 3)) {
        $error = $tr["ERR_LOGIN"];
    } else {
        $pSize = intval($_POST['pSize']);
        if ($pSize < 6 || $pSize > 100)
            $error = $tr["ERR_LOGIN"];
        else
            $password = $_POST['p'];
    }*/
    $email = $_POST['email'];
    $password = $_POST['p'];
    if ($error == "") {
        $ret = login($email, $password, $mysqli);
        if ($ret == LoginResponse::Success) {
            header('Location: ../summary.php');
        } else {
            $error = $tr["ERR_LOGIN"];
        }
    }
}
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
    <h2><?=$tr["LOGIN"]?></h2>

    <div class="formDiv">
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post">
            <label for="email">Email</label><br/>
            <input type="text" name="email" id="email" maxlength="50" value="<?= $email ?>">
            <br/>
            <br/>
            <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
            <input type="password" name="pass" id="pass" maxlength="100">
            <br/>
            <?php if ($error) echo '<span class="errorMessage">' . $error . '</span><br />'; ?>
            <br/>
            <br/>
            <input type="submit" value="<?= $tr["LOGIN"] ?>" class="button"
                   onclick="sendForm(this.form, this.form.pass);">
        </form>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>