<?php
function sec_session_start()
{
    $session_name = 'crouper_s'; // Set a custom session name
    $secure = false; // Set to true if using https.
    $httponly = true; // This stops javascript being able to access the session id.
    ini_set('session.use_only_cookies', 1); // Forces sessions to only use cookies.
    $cookieParams = session_get_cookie_params();
    session_set_cookie_params($cookieParams["lifetime"], $cookieParams["path"], $cookieParams["domain"], $secure, $httponly);
    session_name($session_name); // Sets the session name to the one set above.
    session_start(); // Start the php session
    session_regenerate_id(); // regenerated the session, delete the old one.
}

function login($email, $password, $mysqli)
{
    // Using prepared Statements means that SQL injection is not possible.
    if ($stmt = $mysqli->prepare("SELECT id, username, password, salt FROM accounts WHERE email = ? LIMIT 1")) {
        $stmt->bind_param('s', $email); // Bind "$email" to parameter.
        $stmt->execute(); // Execute the prepared query.
        $stmt->store_result();
        $account_id = NULL;
        $username = NULL;
        $db_password = NULL;
        $salt = NULL;
        $stmt->bind_result($account_id, $username, $db_password, $salt); // get variables from result.
        $stmt->fetch();
        //print("user:" . $username . "<br/> pass: " . $password . "<br/> dbpass: " . $db_password . "<br/>salt: ".$salt);
        $password = hash('sha512', $password . $salt); // hash the password with the unique salt.
        //print("<br/> pass with salt: " . $password);

        if ($stmt->num_rows == 1) { // If the user exists
            // We check if the account is locked from too many login attempts
            if (checkbrute($account_id, $mysqli) == true) {
                // Account is locked
                // Send an email to user saying their account is locked
                return false;
            } else {
                if ($db_password == $password) { // Check if the password in the database matches the password the user submitted.
                    // Password is correct!
                    $user_browser = $_SERVER['HTTP_USER_AGENT']; // Get the user-agent string of the user.

                    $account_id = preg_replace("/[^0-9]+/", "", $account_id); // XSS protection as we might print this value
                    $_SESSION['account_id'] = $account_id;
                    $username = preg_replace("/[^a-zA-Z0-9_\-]+/", "", $username); // XSS protection as we might print this value
                    $_SESSION['username'] = $username;
                    $_SESSION['login_string'] = hash('sha512', $password . $user_browser);
                    return true;
                } else {
                    $now = time();
                    $mysqli->query("INSERT INTO login_attempts (account_id, time) VALUES ('$account_id', '$now')");
                    return false;
                }
            }
        } else {
            // No user exists.
            return false;
        }
    }
}

function checkbrute($account_id, $mysqli)
{
    $now = time();
    $valid_attempts = $now - (2 * 60 * 60);

    if ($stmt = $mysqli->prepare("SELECT time FROM login_attempts WHERE account_id = ? AND time > '$valid_attempts'")) {
        $stmt->bind_param('i', $account_id);
        $stmt->execute();
        $stmt->store_result();
        if ($stmt->num_rows > 5) {
            return true;
        } else {
            return false;
        }
    }
}

function login_check($mysqli)
{
    if (isset($_SESSION['account_id'], $_SESSION['username'], $_SESSION['login_string'])) {
        $account_id = $_SESSION['account_id'];
        $login_string = $_SESSION['login_string'];
        $user_browser = $_SERVER['HTTP_USER_AGENT'];

        if ($stmt = $mysqli->prepare("SELECT password FROM accounts WHERE id = ? LIMIT 1")) {
            $stmt->bind_param('i', $account_id);
            $stmt->execute();
            $stmt->store_result();
            $password = NULL;
            $stmt->bind_result($password);
            $stmt->fetch();
            $login_check = hash('sha512', $password . $user_browser);
            if ($login_check == $login_string) {
                return true; //Logged in!
            } else {
                return false;
            }
        } else {
            dieDb($mysqli);
        }
    } else {
        return false;
    }
}

function initTranslate($mysqli)
{
    global $tr;
    if (!isset($_SESSION["lang"]))
        $_SESSION["lang"] = "en";
    if ($stmt = $mysqli->prepare("SELECT identifier, text FROM strings WHERE language = ?")) {
        $stmt->bind_param('s', $_SESSION["lang"]);
        $stmt->execute();
        $result = $stmt->get_result();
        $arr = $result->fetch_all(MYSQLI_ASSOC);
        foreach ($arr as $row) {
            $tr[$row["identifier"]] = $row["text"];
        }
        $stmt->close();
    }
}

function isValidLang($lang)
{
    if ($lang == "en" || $lang == "hu")
        return true;
    return fal;
}

function needLogin()
{
    global $loggedin;
    if (!$loggedin) {
        header('Location: ./');
        exit();
    }
}

function sanityCheck($string, $type, $lengthmin, $lengthmax)
{

    $type = 'is_' . $type;

    if (!$type($string)) {
        return false;
    } elseif (empty($string)) {
        return false;
    } elseif (strlen($string) < $lengthmin) {
        return false;
    } elseif (strlen($string) > $lengthmax) {
        return false;
    } else {
        return true;
    }
}

function checkEmail($email)
{
    return preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,6})$/', strtolower($email)) ? true : false;
}