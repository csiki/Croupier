<?php
require_once("include.php");
define("NOREPLY_EMAIL", "noreply@poker.sch.bme.hu");
define("FEEDBACK_EMAIL", "poker.platus@gmail.com");

function sendRegisteredEmail($name, $email)
{
    global $tr;
    //get user id from db
    $res = SQL("SELECT id FROM accounts WHERE email = ? and username = ?", $email, $name);
    $id = $res[0]["id"];
    //construct the email
    $to = $email;
    $subject = $tr["REGISTERED_EMAIL_TITLE"];
    //create hash
    $registerHash = md5( rand(0,1000) );
    $registerLink = "http://poker.sch.bme.hu/verify_signup.php?email=".$to."&id=".$registerHash;
    $message = sprintf($tr["REGISTERED_EMAIL_MESSAGE"], $registerLink);
    $from = "\"Croupier poker framework\" <" . NOREPLY_EMAIL . ">";
    $headers = "From:" . $from;
    //store activation hash in db
    /*echo $id."\n\n";
    echo $registerHash."\n\n";*/
    SQL("INSERT INTO account_activation (id, hash, sendTime)
        VALUES (?, ?, NOW());", $id, $registerHash);
    /*echo "\nSUBJECT:".$subject;
    echo "\nTO:".$to;
    echo "\nFROM:".$from;
    echo "\nMSG:".$message;
    echo "\nHEADERS:".$headers;*/
    mail($to,$subject,$message,$headers);
}

function sendFeedbackMail($title, $content)
{
    $res = SQL("SELECT email FROM accounts WHERE id = ?", $_SESSION['accountID']);
    $email = $res[0]["email"];
    $to = FEEDBACK_EMAIL;
    $subject = "#FEEDBACK: ". $title;
    $message = $_SESSION['username'] . " sent feedback.\nEmail: $email\n\n";
    $message .= $content;
    $from = "\"Croupier feedback\" <" . NOREPLY_EMAIL . ">";
    $headers = "From:" . $from;
    mail($to,$subject,$message,$headers);
}