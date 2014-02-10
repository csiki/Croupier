<?php
define("BOT_CODE_MIN", "60");
define("_BOT_AI_RELATIVE_PATH_", "../data/bots/");
define("_GAME_DATA_RELATIVE_PATH_", "../data/games/");
define("_LOG_RELATIVE_PATH_", "../data/logs/");
define("_RESULTS_RELATIVE_PATH_", "../data/results/");

function sec_session_start()
{
    $session_name = 'crouper_s'; // Set a custom session name
    //TODO: set this to true
    $secure = false; // Set to true if using https.
    $httponly = true; // This stops javascript being able to access the session id.
    ini_set('session.use_only_cookies', 1); // Forces sessions to only use cookies.
    $cookieParams = session_get_cookie_params();
    session_set_cookie_params(1200, $cookieParams["path"], $cookieParams["domain"], $secure, $httponly);
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
        } else if ($result->num_rows) {
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
        if (!$metadata) {
            if ($stmt->affected_rows > 0)
                return true;
            return null;
        }
        while (null != ($field = mysqli_fetch_field($metadata))) {
            $fields [] = & $out [$field->name];
        }
        $metadata->close();
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

function check_brute($action, $max_count, $deltaTime)
{
    $login_string = "";
    if (isset($_SESSION['login_string']))
        $login_string = $_SESSION['login_string'];
    $ip = $_SERVER['REMOTE_ADDR'];

    SQL("DELETE FROM brute_force WHERE expires < NOW()");
    $id = substr(base64_encode(sha1($ip . $login_string)), 0, 8);
    $result = SQL("SELECT COUNT(*) FROM brute_force WHERE action = ? AND id = ?", $action, $id);
    if ($result[0]["COUNT(*)"] >= $max_count)
        return false;
    else {
        $date = new DateTime();
        $date->add(new DateInterval('PT' . $deltaTime . 'S'));
        SQL("INSERT INTO brute_force (id, action, expires) VALUES (?, ?, ?)", $id, $action,
            $date->format("Y-m-d H:i:s"));
    }
    return true;
}

function clear_brute($action)
{
    $login_string = "";
    if (isset($_SESSION['login_string']))
        $login_string = $_SESSION['login_string'];
    $ip = $_SERVER['REMOTE_ADDR'];
    $id = substr(base64_encode(sha1($ip . $login_string)), 0, 8);
    SQL("DELETE FROM brute_force WHERE action = ? AND id = ?", $action, $id);
}

function print_captcha()
{
    echo '<script type="text/javascript"
                    src="https://www.google.com/recaptcha/api/challenge?k=6Lev0-kSAAAAAPJsNpRXcA-eLnzOlQD-sLttj2e1">
            </script>
            <noscript>
                <iframe src="https://www.google.com/recaptcha/api/noscript?k=6Lev0-kSAAAAAPJsNpRXcA-eLnzOlQD-sLttj2e1"
                        height="300" width="500" frameborder="0"></iframe><br/>
                <textarea name="recaptcha_challenge_field" rows="3" cols="40"></textarea>
                <input type="hidden" name="recaptcha_response_field" value="manual_challenge"/>
            </noscript>';
}

function login_check(&$loggedin, &$admin)
{
    if (isset($_SESSION['accountID'], $_SESSION['username'], $_SESSION['login_string'])) {
        $accountID = $_SESSION['accountID'];
        $login_string = $_SESSION['login_string'];
        $user_browser = $_SERVER['HTTP_USER_AGENT'];

        $result = SQL("SELECT password, admin FROM accounts WHERE id = ? LIMIT 1", $accountID);
        if ($result == null)
            die("Invalid request"); //the id not exists in the db
        $password = $result[0]["password"];
        $login_check = hash('sha512', $password . $user_browser . getenv("REMOTE_ADDR"));
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
    return preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,6})$/', strtolower($email));
}

function checkUsername($username)
{
    return preg_match('/^[a-zA-Z0-9_]{5,20}$/', $username);
}

function checkBotname($botname)
{
    return preg_match('/^[a-zA-Z0-9_]{5,30}$/', $botname);
}

function getCodeLangID($lang)
{
    switch ($lang) {
        case 'c++':
            return 0;
            break;
        case 'c#':
            return 2;
            break;
        case 'java':
            return 3;
            break;
    }
    //from BotLanguage.h
    /*CPP = 0,
	C = 1,
	CSHARP = 2,
	JAVA = 3,
	PASCAL = 4,
	PROLOG = 5,
	COMMONLISP = 6,
	SCHEME = 7,
	PYTHON = 8,
	PERL = 9,
	LUA = 10,
	RUBY = 11,
	PHP = 12,
	ALIVE = 13*/
}