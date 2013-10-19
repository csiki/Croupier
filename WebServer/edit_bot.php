<?php
include "php/include.php";
needLogin();
$codeErr = $fileErr = "";
$name = $code = $lang = $orig = $id = "";
if (isset($_GET["id"]) && is_numeric($_GET["id"])) {
    $id = $_GET["id"];
    $orig = SQL("SELECT name, code, code_lang FROM bots WHERE accountID = ? && id = ?;", $_SESSION["accountID"], $id);
    if ($orig == null)
        die("Invalid Request");
    $name = $orig[0]["name"];
    $code = $orig[0]["code"];
    $lang = $orig[0]["code_lang"];
} else {
    header('Location: ../manage_bots.php');
    exit();
}
if (isset($_POST["code"]) || isset($_FILES["codefile"]) || isset($_POST["lang"])) {
    if (isset($_POST["name"]) && !empty($_POST["name"]))
        $name = $_POST["name"];
    if (isset($_FILES["codefile"]) && $_FILES["codefile"]["error"] == 0) {
        $code = file_get_contents($_FILES["codefile"]["tmp_name"]);
        $finfo = new finfo(FILEINFO_MIME_TYPE);
        $mimeType = $finfo->buffer($code);
        if (strpos($mimeType, "text/") !== 0 || strlen($code) < BOT_CODE_MIN) {
            $fileErr = $tr["ERR_CODEFILE"];
        }
    } else if (isset($_POST["code"])) {
        $code = $_POST["code"];
        if (strlen($_POST["code"]) < BOT_CODE_MIN)
            $codeErr = $tr["ERR_CODE_EMPTY"];

    }
    if (isset($_POST["lang"]) && isValidCodeLang($_POST["lang"]))
        $lang = $_POST["lang"];
    if ($fileErr == "" && $codeErr == "") {
        SQL("UPDATE bots SET name = ?, lastChangeTime = NOW(), code = ?, code_lang = ?, state = 'processing'
              WHERE id = ?", $name, $code, $lang, $id);
        $res = SQL("SELECT tableName FROM leaderBoards");
        for ($i = 0; $i < count($res); $i++) {
            SQL("DELETE FROM ".$res[$i]["tableName"]." WHERE botID = ?", $id);
        }
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
                theme: "neat",
                mode: "text/x-java"
            });

            $("#codeLang").on("change", langChanged);
        }

        function saveAsk(form) {
            if (messageBoxAsk('<?=$tr["SAVE_BOT_CONF"]?>'))
                form.submit();
        }

        function langChanged() {
            var v = $(this).val();
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
    <h2><?=$tr["EDIT_BOT"]?></h2>

    <p>

    <form action="<?= $_SERVER["PHP_SELF"] . "?id=" . $id ?>" method="post" id="botform" enctype="multipart/form-data">
        <div style="display: inline-block">
            <label for="name"><?=$tr["BOTNAME"]?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>"></div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="codeLang"><?=$tr["CODE_LANG"]?></label><br/>
            <select name="lang" id="codeLang" form="botform">
                <option value="c++" <?php if ($lang == "c++") echo "selected"; ?>>C++</option>
                <option value="java" <?php if ($lang == "java") echo "selected"; ?>>Java</option>
                <option value="c#" <?php if ($lang == "c#") echo "selected"; ?>>C#</option>
            </select>
        </div>
        <br/><br/>

        <div class="codeWrapper">
            <label for="code"><?=$tr["INSERT_CODE"]?></label><br/>
            <textarea cols="80" rows="20" name="code" id="code" style="display: block"
                      wrap="off" autofocus><?= $code ?></textarea>
            <?php if ($codeErr) echo '<span class="errorMessage">' . $codeErr . '</span><br />'; ?>
        </div>
        <br/>
        <label for="codefile"><?=$tr["CHOOSE_FILE_TO"]?></label><br/>
        <input name="codefile" id="codefile" type="file">
        <br/>
        <?php if ($fileErr) echo '<span class="errorMessage">' . $fileErr . '</span><br />'; ?>
        <br/>
        <input type="button" onclick="saveAsk(this.form)" class="button" value="<?= $tr["SAVE"] ?>">
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