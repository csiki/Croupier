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
        Bot name: <input name="name" type="text">
        <br /><br />
        Insert code here:
        <textarea cols="80" rows="20" name="code" style="display: block" wrap="off"></textarea>
        <br /><br />
        OR choose a file to upload: <input name="codefile" type="file">
        <br /><br />
        Language: <select name="lang" form="botform">
            <option value="cpp">C++</option>
            <option value="java">Java</option>
            <option value="csharp">C#</option>
        </select>
        <br /><br />
        <input type="submit" value="Submit">
        </form>
    </p>
</div>
<div id="footer"></div>
</body>
</html>