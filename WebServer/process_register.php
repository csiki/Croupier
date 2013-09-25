<?php
include 'functions.php';
include 'connect_db.php';
sec_session_start();
if(isset($_POST['email'], $_POST['p'], $_POST['name'])) {
    $password = $_POST['p'];
    $random_salt = hash('sha512', uniqid(mt_rand(1, mt_getrandmax()), true));
    // Create salted password (Careful not to over season)
    $password = hash('sha512', $password.$random_salt);
    if ($insert_stmt = $mysqli->prepare("INSERT INTO accounts (username, email, password, salt) VALUES (?, ?, ?, ?)")) {
    $insert_stmt->bind_param('ssss', $username, $email, $password, $random_salt);
    $insert_stmt->execute();
    header('Location: register.php?success=1');
    }
    else
    {
        header('MySQL error');
    }
}
else
{
    echo 'Invalid Request';
}
?>