<?php
require_once "php/include.php";
require_once "php/email.php";
needLogin();
if (!check_brute("sendFeedback", 10, 3600)) {
    echo json_encode($tr["ERR_SEND_FEEDBACK_BRUTE"]);
}
else{
    if (isset($_POST["subject"]) || isset($_POST["text"])) {
        sendFeedbackMail($_POST["subject"], $_POST["text"]);
        echo json_encode($tr["FEEDBACK_THANKS"]);
    }
}