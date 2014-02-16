<?php
require_once "php/include.php";
needLogin();
if (!$admin) {
    header('Location: /');
    exit();
}
if (isset($_GET["id"]) && is_numeric($_GET["id"])) {
    $id = $_GET["id"];
    $orig = SQL("SELECT name, code_lang FROM bots WHERE id = ?;", $id);
    if ($orig == null)
        die("Invalid Request");
    $name = $orig[0]["name"];
    $code = file_get_contents(_BOT_AI_RELATIVE_PATH_ . $id . "/" . $id);
    $lang = $orig[0]["code_lang"];
} else {
    die("Invalid request");
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
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
                readOnly: true
            });
            $("#codeLang").on("change", langChanged);
            langChanged();
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
    <h2>Show bot</h2>
        <div style="display: inline-block">
            <label for="name"><?= $tr["BOTNAME"] ?></label><br/>
            <input name="name" id="name" type="text" value="<?= $name ?>" readonly="true"></div>

        <div style="display: inline-block; margin-left: 40px">
            <label for="codeLang"><?= $tr["CODE_LANG"] ?></label><br/>
            <select name="lang" id="codeLang" form="botform" disabled>
                <option value="c++" <?php if ($lang == "c++") echo "selected"; ?>>C++</option>
                <option value="java" <?php if ($lang == "java") echo "selected"; ?>>Java</option>
                <option value="c#" <?php if ($lang == "c#") echo "selected"; ?>>C#</option>
            </select>
        </div>
        <br/>
        <br/>

        <div class="codeWrapper">
            <label for="code"><?= $tr["INSERT_CODE"] ?></label><br/>
            <textarea cols="80" rows="20" name="code" id="code" style="display: block"
                      wrap="off" autofocus><?= $code ?></textarea>
        </div>
        <br/>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>