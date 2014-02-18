<?php
require_once("include.php");
define("_NOREPLY_EMAIL_", "noreply@poker.sch.bme.hu");

function send_registered_email($name, $email)
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
    $from = "\"Croupier poker framework\" <" . _NOREPLY_EMAIL_ . ">";
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