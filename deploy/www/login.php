<?php
include "php/include.php";
require_once('php/recaptchalib.php');
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$error = $captchaErr = "";
$email = "";
$showCapatcha = false;
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
    $captchaGet = false;
    if (isset($_POST["recaptcha_challenge_field"]) && isset($_POST["recaptcha_challenge_field"])) {
        $captchaGet = true;
        $cap = recaptcha_check_answer("6Lev0-kSAAAAAKT9k5Ie0-1CWuxkywQmg2N8dNWr",
            $_SERVER["REMOTE_ADDR"],
            $_POST["recaptcha_challenge_field"],
            $_POST["recaptcha_response_field"]);
        if (!$cap->is_valid) {
            $captchaErr = $tr["ERR_CAPTCHA"];
            $showCapatcha = true;
        }
    }
    if ($error == "" && (($captchaGet && $captchaErr == "") || !$captchaGet)) {
        if (!check_brute("login", 5, 300)) {
            $showCapatcha = true;
        } else {
            $res = SQL("SELECT id, username, password, salt, activated, lang FROM accounts WHERE email = ? LIMIT 1", $email);
            if (!$res)
                $error = $tr["ERR_LOGIN"];
            else if ($res[0]["activated"] == 0)
                $error = $tr["LOGIN_ACTIVATION_ERR"];
            else {
                $password = hash('sha512', $password . $res[0]["salt"]); // hash the password with the unique salt.
                if ($res[0]["password"] == $password) { // Check if the password in the database matches the password the user submitted.
                    // Password is correct!
                    $user_browser = $_SERVER['HTTP_USER_AGENT']; // Get the user-agent string of the user.
                    $_SESSION['accountID'] = $res[0]["id"];
                    $_SESSION['username'] = $res[0]["username"];
                    $_SESSION["lang"] = $res[0]["lang"];
                    $_SESSION['gravatar'] = md5(strtolower($email));
                    $_SESSION['login_string'] = hash('sha512', $password . $user_browser . getenv("REMOTE_ADDR"));
                    header('Location: ../summary.php');
                    exit();
                } else {
                    $error = $tr["ERR_LOGIN"];
                }
            }

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
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" autocomplete="on">
            <label for="email">Email</label><br/>
            <input type="text" name="email" id="email" maxlength="50" value="<?= $email ?>">
            <br/>
            <br/>
            <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
            <input type="password" name="pass" id="pass" maxlength="100">
            <br/>
            <?php if ($error) echo '<span class="errorMessage">' . $error . '</span><br />'; ?>
            <br/>
            <?php
            if ($showCapatcha) {
                echo '<label for="pass">' . $tr["CAPTCHA"] . '</label><br/>';
                print_captcha();
                if ($captchaErr) echo '<span class="errorMessage">' . $captchaErr . '</span><br />';
            }
            ?>
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