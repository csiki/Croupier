<?php
include "php/include.php";
needLogin();
$codeErr = $fileErr = "";
$name = $code = $lang = "";
if (isset($_POST["code"]) || isset($_FILES["codefile"])) {
    $id = 0;
    $result = SQL("SELECT id FROM bots WHERE accountID = ? ORDER BY id DESC LIMIT 1;", $_SESSION["accountID"]);
    if ($result == null)
        die("Invalid Request");
    $id = $result[0]["id"];
    $id++;
    if (isset($_POST["name"]) && !empty($_POST["name"]))
        $name = $_POST["name"];
    else
        $name = $tr["UNNAMED_BOT"] . " " . $id;
    if (isset($_FILES["codefile"]) && $_FILES["codefile"]["error"] == 0) {
        $code = file_get_contents($_FILES["codefile"]["tmp_name"]);
        $finfo = new finfo(FILEINFO_MIME_TYPE);
        $mimeType = $finfo->buffer($code);
        if (strpos($mimeType, "text/") !== 0 || strlen($code) < BOT_CODE_MIN) {
            $fileErr = $tr["ERR_CODEFILE"];
        }
    } else if (isset($_POST["code"]) && strlen($_POST["code"]) >= BOT_CODE_MIN) {
        $code = $_POST["code"];
    } else {
        $codeErr = $tr["ERR_CODE_EMPTY"];
    }
    if(isset($_POST["lang"]) && isValidCodeLang($_POST["lang"]))
        $lang = $_POST["lang"];
    else
        die("Invalid request");
    if ($fileErr == "" && $codeErr == "") {
        SQL("INSERT INTO bots (id, accountID, name, lastChangeTime, code, code_lang, state)
          VALUES (NULL, ?, ?, NOW(), ?, ?, 'processing')", $_SESSION["accountID"], $name, $code, $lang);
        //update stats
        $res = SQL("SELECT EXISTS(SELECT 1 FROM stat_bots_added WHERE time = CURDATE());");
        if ($res != null)
            SQL("UPDATE stat_bots_added SET count = count + 1 WHERE time = CURDATE();");
        else
            SQL("INSERT INTO stat_bots_added (time, count) VALUES (CURRENT_DATE(), '1')");
        header('Location: ../manage_bots.php');
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/codemirror-compressed.js"></script>
    <link rel="stylesheet" href="scripts/codemirror.css">
    <script type="text/javascript">
        window.onload = function () {
            var myCodeMirror = CodeMirror.fromTextArea(document.getElementById('code'));
        }
    </script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["ADDBOT"]?></h2>

    <p>

    <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" id="botform" enctype="multipart/form-data">
        <div style="display: inline-block">
            <label for="name"><?=$tr["BOTNAME"]?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>"></div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="langInput"><?=$tr["CODE_LANG"]?></label><br/>
            <select name="lang" id="langInput" form="botform">
                <option value="c++" <?php if ($lang == "c++") echo "selected"; ?>>C++</option>
                <option value="java" <?php if ($lang == "java") echo "selected"; ?>>Java</option>
                <option value="c#" <?php if ($lang == "c#") echo "selected"; ?>>C#</option>
            </select>
        </div>
        <br/><br/>
        <label for="code"><?=$tr["INSERT_CODE"]?></label><br/>
        <textarea cols="80" rows="20" name="code" id="code" style="display: block"
                  wrap="off"><?php if (isset($_POST["code"])) echo $_POST["code"]; ?></textarea>
        <?php if ($codeErr) echo '<span class="errorMessage">' . $codeErr . '</span><br />'; ?>
        <br/>
        <label for="codefile"><?=$tr["CHOOSE_FILE_TO"]?></label><br/>
        <input name="codefile" id="codefile" type="file">
        <br/>
        <?php if ($fileErr) echo '<span class="errorMessage">' . $fileErr . '</span><br />'; ?>
        <br/>
        <input type="submit" class="button" value="<?= $tr["SUBMIT"] ?>">
    </form>
    </p>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>