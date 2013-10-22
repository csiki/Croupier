<?php
include "php/include.php";
needLogin();
$codeErr = $fileErr = "";
$name = $code = $lang = $nameErr = "";
if (isset($_POST["code"]) || isset($_FILES["codefile"])) {
    $id = SQL("SHOW TABLE STATUS LIKE 'bots'")[0]["Auto_increment"];
    if (isset($_POST["name"]) && !empty($_POST["name"])) {
        $name = $_POST["name"];
        if (SQL("SELECT * FROM bots WHERE name = ? AND accountID = ?", $name, $_SESSION["accountID"]) != null)
            $nameErr = $tr["ERR_NAME_CONFLICT"];
    } else
        $name = $tr["UNNAMED_BOT"] . " " . $id;
    $codeFileUpload = isset($_FILES["codefile"]) && $_FILES["codefile"]["error"] != UPLOAD_ERR_NO_FILE;
    if ($codeFileUpload) {
        if ($_FILES["codefile"]["error"] == UPLOAD_ERR_OK) {
            //$code = file_get_contents($_FILES["codefile"]["tmp_name"]);
            $finfo = new finfo(FILEINFO_MIME_TYPE);
            $mimeType = $finfo->file($_FILES["codefile"]["tmp_name"]);
            if (strpos($mimeType, "text/") !== 0)
                $fileErr = $tr["ERR_CODEFILE"];
            else if ($_FILES["codefile"]["size"] < 7)
                $fileErr = $tr["ERR_CODE_EMPTY"];
        } else
            die("File upload error: " . $_FILES["codefile"]["error"]);

    } else if (isset($_POST["code"])) {
        if (strlen(isset($_POST["code"]) < BOT_CODE_MIN))
            $fileErr = $tr["ERR_CODE_EMPTY"];
        else
            $code = $_POST["code"];
    }

    if (isset($_POST["lang"]) && isValidCodeLang($_POST["lang"]))
        $lang = $_POST["lang"];
    else
        die("Invalid request");

    if ($fileErr == "" && $codeErr == "" && $nameErr == "") {
        //update database
        SQL("INSERT INTO bots (id, accountID, name, lastChangeTime, code_lang, state)
          VALUES (NULL, ?, ?, NOW(), ?, 'processing')", $_SESSION["accountID"], $name, $lang);

        //create bot folder
        mkdir(_BOT_AI_RELATIVE_PATH_ . $id);

        //move or make file
        if ($codeFileUpload) {
            $tmp_name = $_FILES["codefile"]["tmp_name"];
            move_uploaded_file($tmp_name, _BOT_AI_RELATIVE_PATH_ . $id . "/" . $id);
        } else {
            $ret = file_put_contents(_BOT_AI_RELATIVE_PATH_ . $id . "/" . $id, $code);
            if ($ret === false)
                die("Couldn't write bot to file: " . $id);
        }

        //update stats
        SQL("INSERT INTO stat_bots_added (date, count)
            VALUES (CURRENT_DATE(), 1)
            ON DUPLICATE KEY UPDATE date = VALUES(date), count = count + 1;");

        header('Location: ../manage_bots.php');
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
    <script type="text/javascript" src="scripts/codemirror-compressed.js"></script>
    <script type="text/javascript" src="scripts/matchbrackets.js"></script>
    <script type="text/javascript" src="scripts/clike.js"></script>
    <link rel="stylesheet" href="style/codemirror.css">
    <link rel="stylesheet" href="style/codemirror-neat-modified.css">
    <script type="text/javascript">
        var editor = null;
        window.onload = function () {
            editor = CodeMirror.fromTextArea($("#code").get(0), {
                lineNumbers: true,
                matchBrackets: true,
                theme: "neat"
            });
            $("#codeLang").on("change", langChanged);
            langChanged();
        }

        function saveAsk(form) {
            if (messageBoxAsk('<?=$tr["SAVE_BOT_CONF"]?>'))
                form.submit();
        }

        function langChanged() {
            var v = $("#codeLang").val();
            if (v == "c++")
                editor.setOption("mode", "text/x-c++src");
            else if (v == "c#")
                editor.setOption("mode", "text/x-csharp");
            else if (v == "java")
                editor.setOption("mode", "text/x-java");
        }
    </script>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["NEW_BOT"]?></h2>

    <p>

    <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" id="botform" enctype="multipart/form-data">
        <div style="display: inline-block">
            <label for="name"><?=$tr["BOTNAME"]?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>" autofocus></div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="codeLang"><?=$tr["CODE_LANG"]?></label><br/>
            <select name="lang" id="codeLang" form="botform">
                <option value="c++" <?php if ($lang == "c++") echo "selected"; ?>>C++</option>
                <option value="java" <?php if ($lang == "java") echo "selected"; ?>>Java</option>
                <option value="c#" <?php if ($lang == "c#") echo "selected"; ?>>C#</option>
            </select>
        </div>
        <br/>
        <?=$nameErr ? '<span class="errorMessage">' . $nameErr . '</span><br />' : ''?>
        <br/>

        <div class="codeWrapper">
            <label for="code"><?=$tr["INSERT_CODE"]?></label><br/>
            <textarea cols="80" rows="20" name="code" id="code" style="display: block"
                      wrap="off"><?php if (isset($_POST["code"])) echo $_POST["code"]; ?></textarea>
            <?php if ($codeErr) echo '<span class="errorMessage">' . $codeErr . '</span><br />'; ?>
        </div>
        <br/>
        <label for="codefile"><?=$tr["CHOOSE_FILE_TO"]?></label><br/>
        <input name="codefile" id="codefile" type="file">
        <br/>
        <?php if ($fileErr) echo '<span class="errorMessage">' . $fileErr . '</span><br />'; ?>
        <br/>
        <input type="submit" class="button" value="<?= $tr["NEW_BOT"] ?>">
        <input type="button" onclick="javascript: window.location = '/manage_bots.php';" class="button disabledButton"
               value="<?= $tr["CANCEL"] ?>">
    </form>
    </p>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>