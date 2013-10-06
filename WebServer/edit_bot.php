<?php
include "php/include.php";
needLogin();
if(!isset($_GET["botid"]) || empty($_GET["botid"]))
    die("Invalid Request");

?>
<!DOCTYPE html>
<html>
<head>
<?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["EDITBOT"]?></h2>
    <p>
    <form action="process_add_bot.php" method="post" id="botform" enctype="multipart/form-data">
        <label for="name"><?=$tr["BOTNAME"]?></label><br />
        <input name="name" id="name" type="text">
        <br /><br />
        <label for="code"><?=$tr["INSERT_CODE"]?></label><br />
        <textarea cols="80" rows="20" name="code" id="code" style="display: block" wrap="off"></textarea>
        <br />
        <label for="codefile"><?=$tr["CHOOSE_FILE_TO"]?></label><br />
        <input name="codefile" id="codefile" type="file">
        <br /><br />
        <label for="langInput"><?=$tr["CODE_LANG"]?></label><br />
        <select name="lang" id="langInput" form="botform">
            <option value="cpp">C++</option>
            <option value="java">Java</option>
            <option value="csharp">C#</option>
        </select>
        <br /><br />
        <input type="submit" class="button" value="<?=$tr["SUBMIT"]?>">
    </form>
    </p>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>