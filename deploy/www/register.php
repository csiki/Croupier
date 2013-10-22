<?php
include "php/include.php";
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$nameErr = $passwordErr = $emailErr = "";
$name = $email = "";
if (isset($_POST['name']) || isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pSize'])) {
    if (!isset($_POST['name']) || !sanityCheck($_POST['name'], 'string', 3, 25))
        $nameErr = $tr["ERR_USERNAME_LENGTH"];
    else
    {
        $res = SQL("SELECT 1 FROM accounts WHERE username = ?;", $_POST['name']);
        if($res != null)
            $nameErr = $tr["ERR_USERNAME_EXITS"];
        else
            $name = $_POST['name'];
    }

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
        SQL("INSERT INTO accounts (username, email, password, salt)
        VALUES (?, ?, ?, ?)", $_POST['name'], $_POST['email'], $password, $random_salt);
        //update stats
        SQL("INSERT INTO stat_accounts_added (date, count)
            VALUES (CURRENT_DATE(), 1)
            ON DUPLICATE KEY UPDATE date = VALUES(date), count = count +1;");
        header('Location: ../register.php?success=1');
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
    <h2><?php print($tr["REG_ACCOUNT_TITLE"]); ?></h2>
    <?php
    if (isset($_GET["success"]) && $_GET["success"] == 1)
        echo $tr["REG_THANKS"];
    else {
        ?>
        <div class="formDiv">
            <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post">
                <label for="name"><?=$tr["USERNAME"]?></label><br/>
                <input type="text" name="name" id="name" maxlength="25" value="<?= isset($_POST["name"]) ? $_POST["name"] : "" ?>">
                <br/>
                <?php if ($nameErr) echo '<span class="errorMessage">' . $nameErr . '</span><br />'; ?>
                <br/>
                <label for="email">Email</label><br/>
                <input type="text" name="email" id="email" maxlength="50" value="<?= isset($_POST["email"]) ? $_POST["email"] : ""  ?>">
                <br/>
                <?php if ($emailErr) echo '<span class="errorMessage">' . $emailErr . '</span><br />'; ?>
                <br/>
                <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
                <input type="password" name="pass" id="pass" maxlength="100">
                <br/>
                <?php if ($passwordErr) echo '<span class="errorMessage">' . $passwordErr . '</span><br />'; ?>
                <br/>
                <br/>
                <input type="submit" value="<?= $tr["REGISTER"] ?>" class="button"
                       onclick="sendForm(this.form, this.form.pass);">
            </form>
        </div>
    <?php
    }
    ?>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>