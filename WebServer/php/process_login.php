<?php
include 'functions.php';
include 'connect_db.php';
sec_session_start();
if(isset($_POST['email'], $_POST['p'])) {
    $email = $_POST['email'];
    $password = $_POST['p'];
    if(login($email, $password, $mysqli) == true) {//asd asd
        header('Location: ../summary.php');
    } else {
        header('Location: ../login.php?error=1');
    }
} else {
    die('Invalid Request');
}