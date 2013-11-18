<?php
include "php/include.php";
require_once('php/recaptchalib.php');
require_once('php/email.php');
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$nameErr = $passwordErr = $emailErr = $captchaErr = "";
$name = $email = "";
if (isset($_POST['name']) || isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pSize'])) {
    if (!isset($_POST['name']) || !sanityCheck($_POST['name'], 'string', 3, 25))
        $nameErr = $tr["ERR_USERNAME_LENGTH"];
    else {
        $res = SQL("SELECT 1 FROM accounts WHERE username = ?;", $_POST['name']);
        //TODO: warning when wrong format
        if ($res != null)
            $nameErr = $tr["ERR_USERNAME_EXITS"];
        else
            $name = xssafe($_POST['name']);
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

    $cap = recaptcha_check_answer("6Lev0-kSAAAAAKT9k5Ie0-1CWuxkywQmg2N8dNWr",
        $_SERVER["REMOTE_ADDR"],
        $_POST["recaptcha_challenge_field"],
        $_POST["recaptcha_response_field"]);

    if (!$cap->is_valid)
        $captchaErr = $tr["ERR_CAPTCHA"];

    if ($nameErr == "" && $passwordErr == "" && $emailErr == "" && $captchaErr == "") {
        $random_salt = hash('sha512', uniqid(mt_rand(1, mt_getrandmax()), true));
        // Create salted password (Careful not to over season)
        $password = hash('sha512', $password . $random_salt);
        SQL("INSERT INTO accounts (username, email, password, salt, lang)
        VALUES (?, ?, ?, ?, ?)", $name, $email, $password, $random_salt, $_SESSION["lang"]);
        send_registered_email($name, $email);
        //update stats
        SQL("INSERT INTO stat_accounts_added (date, count)
            VALUES (CURRENT_DATE(), 1)
            ON DUPLICATE KEY UPDATE date = VALUES(date), count = count + 1;");

        header('Location: ../register.php?success=1&email=' . $email);
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/sha512.js"></script>
    <script>
        $(function () {
            $("#submitButton").click(function () {
                if (checkMatch())
                    sendForm($("#register_form").get(0), $("#pass").get(0));
            });
            setInterval(checkMatch, 500);
            $("[title]").tooltip({position: {
                my: "center bottom-20",
                at: "right top"}});
        });
        function checkMatch() {
            $("#pass_mismatch").hide();
            if ($("#pass_again").val() != $("#pass").val()) {
                $("#pass_mismatch").show();
                return false;
            }
            return true;
        }
    </script>

</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?php print($tr["REG_ACCOUNT_TITLE"]); ?></h2>
    <?php
    if (isset($_GET["success"]) && $_GET["success"] == 1 && isset($_GET["email"]))
        echo sprintf($tr["REGISTER_THANKS"], $_GET["email"]);
    else {
        ?>
        <div class="formDiv">
            <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" id="register_form" autocomplete="off">
                <label for="name"><?=$tr["USERNAME"]?></label><br/>
                <input type="text" name="name" id="name" maxlength="25"
                       value="<?= isset($_POST["name"]) ? $_POST["name"] : "" ?>">
                <br/>
                <?php if ($nameErr) echo '<span class="errorMessage">' . $nameErr . '</span><br />'; ?>
                <br/>
                <label for="email">Email</label><br/>
                <input type="text" name="email" id="email" maxlength="50"
                       value="<?= isset($_POST["email"]) ? $_POST["email"] : "" ?>">
                <br/>
                <?php if ($emailErr) echo '<span class="errorMessage">' . $emailErr . '</span><br />'; ?>
                <br/>
                <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
                <input type="password" name="pass" id="pass" maxlength="100" title="<?= $tr["ERR_PASSWORD_LENGTH"] ?>">
                <br/><br/>
                <label for="pass"><?=$tr["PASSWORD_AGAIN"]?></label><br/>
                <input type="password" id="pass_again" maxlength="100">
                <br/>

                <div id="pass_mismatch" style="display: none;"><span class="errorMessage"
                                                                     id="pass_mismatch"><?=$tr["PASS_NOT_MATCH"]?></span>
                    <br/></div>
                <?php if ($passwordErr) echo '<span class="errorMessage">' . $passwordErr . '</span><br />'; ?>
                <br/>
                <label for="pass"><?=$tr["CAPTCHA"]?></label><br/>
                <?=print_captcha()?>
                <?php if ($captchaErr) echo '<span class="errorMessage">' . $captchaErr . '</span><br />'; ?>
                <br/>
                <br/>
                <input type="button" value="<?= $tr["REGISTER"] ?>" class="button" id="submitButton">
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