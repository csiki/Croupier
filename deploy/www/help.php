<?php
require_once "php/include.php";
needLogin();
?>
<!DOCTYPE html>
<html>
<head>
    <?php require "php/head.php"; ?>
    <script>
        $(function () {
            $("#sendFeedback").click(function(){
                $.post( "send_feedback.php", $( "#feedbackForm" ).serialize(), function( data ) {
                    messageBox(data);
                    $("#subject").val("");
                    $("#text").val("");
                }, "json");
            });
        });

    </script>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["HELP"] ?></h2>
    <?php
    echo '<span style="font-size: 22px">' .sprintf($tr["HELP_MAIN"], "docs/mi_dev/miDev.html") ."</span>";
    echo '<span style="font-size: 22px; display: block; margin-top: 50px">';
    echo sprintf($tr["FOOTER_TEXT"], '<a href="mailto:poker.platus@gmail.com">poker.platus@gmail.com</a>');
    echo '</span>';
    ?>
    <h2 style="display: block; margin-top: 50px"><?= $tr["FEEDBACK"] ?></h2>
    <form id="feedbackForm">
        <div>
            <label for="subject"><?= $tr["SUBJECT"] ?></label><br/>
            <input name="subject" id="subject" type="text">
        </div>
        <br/>
            <label for="text"><?= $tr["BODY"] ?></label><br/>
            <textarea name="text" id="text" style="display: block; width: 600px; height: 250px;"
                      wrap="off"></textarea>
        <br/>
        <input type="button" class="button" id="sendFeedback" value="<?= $tr["SEND_FEEDBACK"] ?>">
    </form>
</div>
<footer>
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>