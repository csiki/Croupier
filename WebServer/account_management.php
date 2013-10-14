<?php
include "php/include.php";
needLogin();
$nameErr = $passwordErr = $emailErr = "";
$name = $email = "";
if (isset($_POST['name']) || isset($_POST['email']) || isset($_POST['p']) || isset($_POST['pSize'])) {
    if (isset($_POST["name"]) && !empty($_POST["name"]))
        $name = $_POST["name"];
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
    if (isset($_POST["lang"]) && isValidCodeLang($_POST["lang"]))
        $lang = $_POST["lang"];
    if ($fileErr == "" && $codeErr == "") {
        SQL("UPDATE bots SET name = ?, lastChangeTime = NOW(), code = ?, code_lang = ?, state = 'processing',
          participate = '0' WHERE id = ?", $name, $code, $lang, $id);
        header('Location: ../manage_bots.php');
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["ACCOUNT_MANAGEMENT"]?> - <?=$_SESSION["username"]?></h2>

    <div class="formDiv">
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post">
            <label for="name"><?=$tr["USERNAME"]?></label><br/>
            <input type="text" name="name" id="name" maxlength="25"
                   value="<?=$_SESSION["username"]?>">
            <br/>
            <?php if ($nameErr) echo '<span class="errorMessage">' . $nameErr . '</span><br />'; ?>
            <br/>
            <label for="email">Email</label><br/>
            <input type="text" name="email" id="email" maxlength="50"
                   value="<?= isset($_POST["email"]) ? $_POST["email"] : "" ?>">
            <br/>
            <?php if ($emailErr) echo '<span class="errorMessage">' . $emailErr . '</span><br />'; ?>
            <br/>
            <label for="pass"><?=$tr["PASSWORD"]?></label><br/>
            <input type="password" name="pass" id="pass" maxlength="100">
            <br/>
            <?php if ($passwordErr) echo '<span class="errorMessage">' . $passwordErr . '</span><br />'; ?>
            <br/>
            <br/>
            <input type="submit" value="<?= $tr["REGISTER"] ?>" class="button"
                   onclick="sendForm(this.form, this.form.pass);">
        </form>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>