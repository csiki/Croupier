<?php

define("BOT_CODE_MIN", "60");
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

function SQL($Query)
{
    global $DB;
    $args = func_get_args();
    if (count($args) == 1) {
        $result = $DB->query($Query);
        if ($result == false)
            dieDb();
        else if ($result === true) {
            return true;
        }else if ($result->num_rows) {
            return $result->fetch_all(MYSQLI_ASSOC);
        }
        return null;
    } else {
        if (!$stmt = $DB->prepare($Query))
            dieDb();
        array_shift($args); //remove $Query from args
        //the following three lines are the only way to copy an array values in PHP
        $a = array();
        foreach ($args as $k => &$v)
            $a[$k] = & $v;
        $types = str_repeat("s", count($args)); //all params are strings, works well on MySQL and SQLite
        array_unshift($a, $types);
        call_user_func_array(array($stmt, 'bind_param'), $a);
        $stmt->execute();
        //fetching all results in a 2D array
        $metadata = $stmt->result_metadata();
        $out = array();
        $fields = array();
        if (!$metadata)
            return null;
        while (null != ($field = mysqli_fetch_field($metadata))) {
            $fields [] = & $out [$field->name];
        }
        call_user_func_array(array(
            $stmt, "bind_result"
        ), $fields);
        $output = array();
        $count = 0;
        while ($stmt->fetch()) {
            foreach ($out as $k => $v)
                $output [$count] [$k] = $v;
            $count++;
        }
        $stmt->free_result();
        return ($count == 0) ? null : $output;
    }
}

function xssafe($data, $encoding = 'UTF-8')
{
    return htmlspecialchars($data, ENT_QUOTES | ENT_HTML401, $encoding);
}

class LoginResponse
{
    const Success = 0;
    const NotFound = 1;
    const Brute = 2;
    const WrongPass = 3;
}

function login($email, $password, $mysqli)
{
    $result = SQL("SELECT id, username, password, salt FROM accounts WHERE email = ? LIMIT 1", $email);
    if ($result == null) //not found
        return LoginResponse::NotFound;
    $accountID = $result[0]["id"];
    $username = $result[0]["username"];
    $db_password = $result[0]["password"];
    $salt = $result[0]["salt"];
    $password = hash('sha512', $password . $salt); // hash the password with the unique salt.
    if (checkbrute($accountID, $mysqli) == true) {
        // Account is locked
        // Send an email to user saying their account is locked
        return LoginResponse::Brute;
    } else {
        if ($db_password == $password) { // Check if the password in the database matches the password the user submitted.
            // Password is correct!
            $user_browser = $_SERVER['HTTP_USER_AGENT']; // Get the user-agent string of the user.
            $accountID = preg_replace("/[^0-9]+/", "", $accountID); // XSS protection as we might print this value
            $_SESSION['accountID'] = $accountID;
            $username = preg_replace("/[^a-zA-Z0-9_\-]+/", "", $username); // XSS protection as we might print this value
            $_SESSION['username'] = $username;
            $_SESSION['login_string'] = hash('sha512', $password . $user_browser);
            return LoginResponse::Success;
        } else {
            $now = time();
            SQL("INSERT INTO login_attempts (accountID, time) VALUES ('$accountID', '$now')");
            return LoginResponse::WrongPass;
        }
    }
}

function checkbrute($accountID, $mysqli)
{
    $now = time();
    $valid_attempts = $now - (2 * 60 * 60);

    if ($result = SQL("SELECT time FROM login_attempts WHERE accountID = ? AND time > '$valid_attempts'", $accountID)) {
        if ($result == null) {
            return true;
        } else if (count($result) > 5) {
            return true;
        } else {
            return false;
        }
    }
}

function login_check(&$loggedin, &$admin)
{
    if (isset($_SESSION['accountID'], $_SESSION['username'], $_SESSION['login_string'])) {
        $accountID = $_SESSION['accountID'];
        $login_string = $_SESSION['login_string'];
        $user_browser = $_SERVER['HTTP_USER_AGENT'];

        $result = SQL("SELECT password,  admin FROM accounts WHERE id = ? LIMIT 1", $accountID);
        if ($result == null)
            die("Invalid request"); //the id not exists in the db
        $password = $result[0]["password"];
        $login_check = hash('sha512', $password . $user_browser);
        if ($login_check == $login_string) {
            //logged in
            SQL("UPDATE accounts SET lastOnline=NOW() WHERE id = ?", $accountID); //set lastOnline
            $loggedin = true;
            $admin = $result[0]["admin"] == "1";
        }
    }
}

function initTranslate()
{
    global $tr;
    if (!isset($_SESSION["lang"]))
        $_SESSION["lang"] = "en";
    if ($result = SQL("SELECT identifier, text FROM strings WHERE language = ?", $_SESSION["lang"])) {
        foreach ($result as $row) {
            $tr[$row["identifier"]] = $row["text"];
        }
    }
}

function isValidLang($lang)
{
    if ($lang == "en" || $lang == "hu")
        return true;
    return false;
}

function isValidCodeLang($lang)
{
    if ($lang == "c++" || $lang == "java" || $lang == "c#")
        return true;
    return false;
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
