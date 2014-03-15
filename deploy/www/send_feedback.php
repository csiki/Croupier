<?php
require_once "php/include.php";
needLogin();
if (!check_brute("sendFeedback", 10, 3600)) {
    echo json_encode($tr["ERR_SEND_FEEDBACK_BRUTE"]);
}
else{
    if (isset($_POST["subject"]) || isset($_POST["text"])) {
        $res = SQL("INSERT INTO feedbacks
            (id, subject, body, accountID, username, time) VALUES (NULL, ?, ?, ?, ?, NOW());",
            $_POST["subject"], $_POST["text"], $_SESSION["accountID"], $_SESSION["username"]);
        echo json_encode($tr["FEEDBACK_THANKS"]);
    }
}