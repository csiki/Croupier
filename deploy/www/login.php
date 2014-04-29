<?php
require_once "php/include.php";
require_once('php/recaptchalib.php');
if ($loggedin) {
    header('Location: summary.php');
    exit();
}
$errors = array();
$email = "";
$showCaptcha = false;
if (isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pLength'])) {
    //form validation
    $email = $_POST['email'];
    if (!isset($_POST['pLength']) || !sanityCheck($_POST['pLength'], 'numeric', 0, 3)) {
        $errors[] = $tr["ERR_LOGIN"];
    } else {
        $pLength = intval($_POST['pLength']);
        if ($pLength < 6 || $pLength > 100)
            $errors[] = $tr["ERR_LOGIN"];
        else
            $password = $_POST['p'];
    }
    $password = $_POST['p'];
    $captchaGet = false;
    $captchaWrong = false;
    if (isset($_POST["recaptcha_challenge_field"]) && isset($_POST["recaptcha_challenge_field"])) {
        $captchaGet = true;
        $cap = recaptcha_check_answer("6Lev0-kSAAAAAKT9k5Ie0-1CWuxkywQmg2N8dNWr",
            $_SERVER["REMOTE_ADDR"],
            $_POST["recaptcha_challenge_field"],
            $_POST["recaptcha_response_field"]);
        if (!$cap->is_valid) {
            $captchaWrong = true;
            $showCaptcha = true;
            $errors[] = $tr["ERR_CAPTCHA"];
        }
    }

    if ((count($errors) == 0) && (($captchaGet && !$captchaWrong) || !$captchaGet)) {
        $res = SQL("SELECT id, username, password, salt, activated, lang, email FROM accounts WHERE email = ? OR username = ? LIMIT 1", $email, $email);
        if ($res == null) {
            $errors[] = $tr["ERR_LOGIN"];
            if (!check_brute("login", 5, 300)) {
                $showCaptcha = true; //show captcha after 5 try
            }
        }
        else if ($res[0]["activated"] == 0) {
            $errors[] = $tr["LOGIN_ACTIVATION_ERR"];
            if (!check_brute("login", 5, 300)) {
                $showCaptcha = true; //show captcha after 5 try
            }
        } else {
            $password = hash('sha512', $password . $res[0]["salt"]);
            if ($res[0]["password"] == $password) { // Check if the password in the database matches the password the user submitted.
                // Password is correct!
                clear_brute("login");
                $user_browser = $_SERVER['HTTP_USER_AGENT']; // Get the user-agent string of the user.
                $_SESSION['accountID'] = $res[0]["id"];
                $_SESSION['username'] = $res[0]["username"];
                $_SESSION["lang"] = $res[0]["lang"];
                $_SESSION['gravatar'] = md5(strtolower($res[0]["email"]));
                $_SESSION['login_string'] = hash('sha512', $password . $user_browser . getenv("REMOTE_ADDR"));
                header('Location: ../summary.php');
                exit();
            } else {
                $errors[] = $tr["ERR_LOGIN"];
                if (!check_brute("login", 5, 300)) {
                    $showCaptcha = true; //show captcha after 5 try
                }
            }
        }
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script type="text/javascript" src="scripts/sha512.js"></script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["LOGIN"] ?></h2>

    <div class="formDiv">
        <?php
        foreach ($errors as $error) {
            echo '<span class="errorMessage">' . $error . '</span><br />';
        }
        ?>
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" autocomplete="on">
            <label for="email"><?= $tr["EMAIL_OR_USERNAME"] ?></label><br/>
            <input autocomplete="on" type="text" name="email" id="email" maxlength="50" value="<?= $email ?>">
            <br/>
            <br/>
            <label for="pass"><?= $tr["PASSWORD"] ?></label><br/>
            <input autocomplete="on" type="password" name="pass" id="pass" maxlength="100">
            <br/>
            <br/>
            <?php
            if ($showCaptcha) {
                echo '<label for="pass">' . $tr["CAPTCHA"] . '</label><br/>';
                print_captcha();
            }
            ?>
            <br/>
            <input type="submit" value="<?= $tr["LOGIN"] ?>" class="button"
                   onclick="sendForm(this.form, this.form.pass);">
        </form>
    </div>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>