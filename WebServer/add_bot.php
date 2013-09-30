<?php
include "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2><?=$tr["ADDBOT"]?></h2>
    <p>
        <form action="process_add_bot.php" method="post" id="botform" enctype="multipart/form-data">
        <label for="name">Bot name</label><br />
        <input name="name" id="name" type="text">
        <br /><br />
        <label for="code">Insert code here</label><br />
        <textarea cols="80" rows="20" name="code" id="code" style="display: block" wrap="off"></textarea>
        <br />
        <label for="codefile">OR choose a file to upload</label><br />
            <input name="codefile" id="codefile" type="file">
        <br /><br />
        <label for="langInput">Language</label><br />
        <select name="lang" id="langInput" form="botform">
            <option value="cpp">C++</option>
            <option value="java">Java</option>
            <option value="csharp">C#</option>
        </select>
        <br /><br />
        <input type="submit" class="button" value="Submit">
        </form>
    </p>
</div>
<div id="footer"></div>
</body>
</html>