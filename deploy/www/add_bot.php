<?php
require_once "php/include.php";
needLogin();
$errors = array();
$name = $className = $code = $lang = "";
if (isset($_POST["code"]) || isset($_FILES["codefile"])) {
    $res = SQL("SHOW TABLE STATUS LIKE 'bots'");
    $id = $res[0]["Auto_increment"];
    if (isset($_POST["name"]) && !empty($_POST["name"])) {
        if (!sanityCheck($_POST['name'], 'string', 5, 30))
            $errors[] = $tr["ERR_BOTNAME_LENGTH"];
        else if (!checkBotname($_POST['name']))
            $errors[] = $tr["ERR_BOTNAME_FORMAT"];
        else if (SQL("SELECT 1 FROM bots WHERE name = ?", $name) != null)
            $errors[] = $tr["ERR_NAME_CONFLICT"];
        else
            $name = xssafe($_POST['name']);
    } else
        $name = $tr["UNNAMED_BOT"] . " " . $id;

    if (isset($_POST["className"]) && !empty($_POST["className"])) {
        if (!sanityCheck($_POST['className'], 'string', 1, 60))
            $errors[] = $tr["ERR_BOTCLASSNAME_LENGTH"];
        else
            $className = $_POST['className'];
    } else
        $errors[] = $tr["ERR_BOTCLASSNAME_LENGTH"];

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
    if (!check_brute("addBot", 30, 3600)) {
        $errors[] = $tr["ERR_ADDBOT_BRUTE"];
    } else if (count($errors) == 0) {
        //update database
        SQL("INSERT INTO bots (id, accountID, name, className, lastChangeTime, code_lang, state)
              VALUES (NULL, ?, ?, ?, NOW(), ?, 'processing')", $_SESSION["accountID"], $name, $className, $lang);

        //create account folder if not exits
        $dir = _BOT_AI_RELATIVE_PATH_ . $_SESSION['accountID'] . '/';
        if (!is_dir($dir)) {
            $ret = true;
            $ret = mkdir($dir);
            chmod($dir, 0770);
            if ($ret === false) {
                SQL("DELETE FROM bots WHERE id = ?", $id); //remove db entry
                die("Couldn't create folder for bot: " . $name);
            }
        }
        //move or make file
        if ($codeFileUpload) {
            $tmp_name = $_FILES["codefile"]["tmp_name"];
            $newFileName = $dir . $id. ".cpp";
            move_uploaded_file($tmp_name, $newFileName);
            chmod($newFileName, 0770);
        } else {
            $fileName = $dir . $id . ".cpp";
            $ret = file_put_contents($fileName, $code);
            chmod($fileName, 0770);
            if ($ret === false) {
                SQL("DELETE FROM bots WHERE id = ?", $id); //remove db entry
                die("Couldn't write bot to file: " . $name);
            }
        }

        //update stats
        SQL("INSERT INTO stat_bots_added (date, count)
                VALUES (CURRENT_DATE(), 1)
                ON DUPLICATE KEY UPDATE date = VALUES(date), count = count + 1;");

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
    <h2><?= $tr["CREATE_BOT"] ?></h2>
    <?php
    foreach ($errors as $error) {
        echo '<span class="errorMessage">' . $error . '</span><br />';
    }
    ?>
    <p>

    <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post" id="botform" enctype="multipart/form-data">
        <div style="display: inline-block">
            <label for="name"><?= $tr["BOTNAME"] ?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>" autofocus>
        </div>

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
                      wrap="off"><?php if (isset($_POST["code"])) echo $_POST["code"]; ?></textarea>
        </div>
        <br/>
        <label for="codefile"><?= $tr["CHOOSE_FILE_TO"] ?></label><br/>
        <input name="codefile" id="codefile" type="file">
        <br/>
        <br/>
        <input type="button" onclick="cancelAsk()" class="disabledButton button" value="<?= $tr["CANCEL"] ?>">
        <input type="submit" class="button" value="<?= $tr["SAVE"] ?>">
    </form>
    </p>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>