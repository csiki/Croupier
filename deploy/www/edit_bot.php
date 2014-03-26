<?php
require_once "php/include.php";
needLogin();
$errors = array();
$name = $className = $code = $lang = $orig = $id = "";
if (isset($_GET["id"]) && is_numeric($_GET["id"])) {
    $id = $_GET["id"];
    $orig = SQL("SELECT name, className, code_lang FROM bots WHERE accountID = ? AND id = ?;", $_SESSION["accountID"], $id);
    if ($orig == null)
        die("Invalid Request");
    $name = $orig[0]["name"];
    $className = $orig[0]["className"];
    $code = file_get_contents(_BOT_AI_RELATIVE_PATH_ . $_SESSION["accountID"] . "/" . $id.".cpp");
    $lang = $orig[0]["code_lang"];
} else {
    die("Invalid request");
}
if (isset($_POST["code"]) || isset($_FILES["codefile"]) || isset($_POST["lang"])) {
    if (isset($_POST["name"]) && !empty($_POST["name"]))
        $name = $_POST["name"];
    if (SQL("SELECT * FROM bots WHERE name = ? AND id != ?", $name, $id) != null)
        $errors[] = $tr["ERR_NAME_CONFLICT"];

    if (isset($_POST["className"]) && !empty($_POST["className"])) {
        if (!sanityCheck($_POST['className'], 'string', 1, 60))
            $errors[] = $tr["ERR_BOTCLASSNAME_LENGTH"];
        else
            $className = $_POST['className'];
    }

    $codeFileUpload = isset($_FILES["codefile"]) && $_FILES["codefile"]["error"] != UPLOAD_ERR_NO_FILE;
    if ($codeFileUpload) {
        if ($_FILES["codefile"]["error"] == UPLOAD_ERR_OK) {
            //$code = file_get_contents($_FILES["codefile"]["tmp_name"]);
            $finfo = new finfo(FILEINFO_MIME_TYPE);
            $mimeType = $finfo->file($_FILES["codefile"]["tmp_name"]);
            if (strpos($mimeType, "text/") !== 0)
                $errors[] = $tr["ERR_CODEFILE"];
            else if ($_FILES["codefile"]["size"] < BOT_CODE_MIN)
                $errors[] = $tr["ERR_CODE_SHORT"];
        } else
            die("File upload error: " . $_FILES["codefile"]["error"]);

    } else if (isset($_POST["code"])) {
        if (strlen($_POST["code"]) < BOT_CODE_MIN)
            $errors[] = $tr["ERR_CODE_SHORT"];
        else
            $code = $_POST["code"];
    }

    if (isset($_POST["lang"]) && isValidCodeLang($_POST["lang"]))
        $lang = $_POST["lang"];
    else
        die("Invalid request");

    if (count($errors) == 0) {
        //update bots table
        SQL("UPDATE bots SET name = ?, className = ?, lastChangeTime = NOW(), code_lang = ?, state = 'processing'
              WHERE id = ?", $name, $className, $lang, $id);

        $fileName = _BOT_AI_RELATIVE_PATH_ . $_SESSION["accountID"] . "/" . $id.".cpp";

        //remove previous file
        unlink($fileName);

        //move or overwrite file
        if ($codeFileUpload) {
            $tmp_name = $_FILES["codefile"]["tmp_name"];
            move_uploaded_file($tmp_name, $fileName);
        } else {
            $ret = file_put_contents($fileName, $code);
            if ($ret === false)
                die("Couldn't write bot to file: " . $id);
        }

        //remove bot from leaderboards
        $leaderBoardTables = SQL("SELECT tableName FROM leaderboards");
        for ($i = 0; $i < count($leaderBoardTables); $i++) {
            SQL("DELETE FROM " . $leaderBoardTables[$i]["tableName"] . " WHERE botID = ?", $id);
        }

        //remove bot from games_by_bots
        SQL("DELETE FROM games_by_bots WHERE botID = ?", $id);

        header('Location: ../my_bots.php');
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script type="text/javascript" src="scripts/codemirror-compressed.js"></script>
    <link rel="stylesheet" href="style/codemirror.css">
    <link rel="stylesheet" href="style/codemirror-neat-modified.css">
    <script type="text/javascript">
        var editor = null;
        $(function () {
            editor = CodeMirror.fromTextArea($("#code").get(0), {
                lineNumbers: true,
                matchBrackets: true,
                theme: "neat"
            });
            $("#codeLang").on("change", langChanged);
            langChanged();
        });

        function saveAsk(form) {
            messageBoxYesNo("<?=$tr["SAVE_BOT_CONF"]?>", "<?=$tr["YES"]?>", "<?=$tr["NO"]?>", function () {
                form.submit();
            });
        }

        function cancelAsk() {
            if (editor.getValue().length != 0 || $("#codefile").val().length != 0)
                messageBoxYesNo("<?=$tr["CANCEL_BOT_CONF"]?>", "<?=$tr["YES"]?>", "<?=$tr["NO"]?>", function () {
                    window.history.back();
                });
            else
                window.history.back();
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
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["EDIT_BOT"] ?></h2>

    <?php
    foreach ($errors as $error) {
        echo '<span class="errorMessage">' . $error . '</span><br />';
    }
    ?>
    <p>

    <form action="<?= $_SERVER["PHP_SELF"] . "?id=" . $id ?>" method="post" id="botform" enctype="multipart/form-data">
        <div style="display: inline-block">
            <label for="name"><?= $tr["BOTNAME"] ?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>"></div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="name"><?= $tr["BOTCLASSNAME"] ?></label><br/>
            <input name="className" id="className" type="text" value="<?= $className ?>">
        </div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="codeLang"><?= $tr["CODE_LANG"] ?></label><br/>
            <select name="lang" id="codeLang" form="botform">
                <option value="c++" <?php if ($lang == "c++") echo "selected"; ?>>C++</option>
<!--                <option value="java" --><?php //if ($lang == "java") echo "selected"; ?><!-->Java</option>-->
<!--                <option value="c#" --><?php //if ($lang == "c#") echo "selected"; ?><!-->C#</option>-->
            </select>
        </div>
        <br/>
        <br/>

        <div class="codeWrapper">
            <label for="code"><?= $tr["INSERT_CODE"] ?></label><br/>
            <textarea name="code" id="code" style="display: block"
                      wrap="off" autofocus><?= $code ?></textarea>
        </div>
        <br/>
        <label for="codefile"><?= $tr["CHOOSE_FILE_TO"] ?></label><br/>
        <input name="codefile" id="codefile" type="file">
        <br/>
        <br/>
        <input type="button" onclick="cancelAsk()" class="disabledButton button" value="<?= $tr["CANCEL"] ?>">
        <input type="button" onclick="saveAsk(this.form)" class="button" value="<?= $tr["SAVE"] ?>">
    </form>
    </p>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>