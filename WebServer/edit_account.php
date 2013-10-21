<?php
include "php/include.php";
needLogin();
$nameErr = $passwordErr = $emailErr = "";
$id = $name = $email = "";
//propagate default values
$id = $_SESSION["accountID"];
$name = $_SESSION["username"];
$emailRes = SQL("SELECT email FROM accounts WHERE id = ?", $id);
if ($emailRes != null)
    $email = $emailRes[0]["email"];
//set post values
if (isset($_POST['name']) || isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pSize'])) {
    if (!isset($_POST['name']) || !sanityCheck($_POST['name'], 'string', 3, 25))
        $nameErr = $tr["ERR_USERNAME_LENGTH"];
    else
        $name = $_POST['name'];

    if (!isset($_POST['email']) || !sanityCheck($_POST['email'], 'string', 7, 50) || !checkEmail($_POST['email']))
        $emailErr = $tr["ERR_EMAIL"];
    else
        $email = $_POST['email'];

    if (!isset($_POST['pSize']) || !sanityCheck($_POST['pSize'], 'numeric', 0, 3)) {
        $passwordErr = $tr["ERR_PASSWORD_LENGTH"];
    } else {
        $pSize = intval($_POST['pSize']);
        if ($pSize < 6 || $pSize > 100)
            $passwordErr = $tr["ERR_PASSWORD_LENGTH"];
        else
            $password = $_POST['p'];
    }
    if ($nameErr == "" && $passwordErr == "" && $emailErr == "") {
        $random_salt = hash('sha512', uniqid(mt_rand(1, mt_getrandmax()), true));
        // Create salted password (Careful not to over season)
        $password = hash('sha512', $password . $random_salt);
        SQL("UPDATE accounts SET username = ?, email = ?, password = ?, salt = ? WHERE id = ?",
            $name, $email, $password, $random_salt, $id);
        //update session values like in Login()
        $user_browser = $_SERVER['HTTP_USER_AGENT'];
        $_SESSION['accountID'] = $id;
        $_SESSION['username'] = $name;
        $_SESSION['login_string'] = hash('sha512', $password . $user_browser);
        header('Location: ../'.$_SERVER["PHP_SELF"].'?success=1');
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/sha512.js"></script>
    <script>
        $( document ).ready(function() {
            var get = getQueryParams(document.location.search);
            if(get['success'] !== undefined)
                messageBox('<?=$tr["ACCOUNT_SAVED"]?>');
        });
    </script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["ACCOUNT_MANAGEMENT"]?> - <?=$_SESSION["username"]?></h2>

    <div class="formDiv">
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post">
            <label for="name"><?=$tr["NAME"]?></label><br/>
            <input type="text" name="name" id="name" maxlength="25"
                   value="<?=$name ?>">
            <br/>
            <?php if ($nameErr) echo '<span class="errorMessage">' . $nameErr . '</span><br />'; ?>
            <br/>
            <label for="email">Email</label><br/>
            <input type="text" name="email" id="email" maxlength="50"
                   value="<?=$email?>">
            <br/>
            <?php if ($emailErr) echo '<span class="errorMessage">' . $emailErr . '</span><br />'; ?>
            <br/>
            <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
            <input type="password" name="pass" id="pass" maxlength="100">
            <br/>
            <?php if ($passwordErr) echo '<span class="errorMessage">' . $passwordErr . '</span><br />'; ?>
            <br/>
            <br/>
            <input type="submit" value="<?= $tr["SAVE"] ?>" class="button"
                   onclick="sendForm(this.form, this.form.pass);">
        </form>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>