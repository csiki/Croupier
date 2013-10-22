<?php
include "php/include.php";
needLogin();
if (!$admin) {
    header('Location: ./');
    exit();
}
if (isset($_POST["title"]) || isset($_POST["content"])) {
    $res = SQL("INSERT INTO news_posts
        (id, title, author, date, content) VALUES (NULL, ?, ?, NOW(), ?);",
        $_POST["title"], $_SESSION["username"], $_POST["content"]);
    echo json_encode('Posted "'.$_POST["title"].'"');
}