<?php
include "php/include.php";
$currentPage = 0;
$itemsPerPage = 10;
if (isset($_GET["page"]) && !empty($_GET["page"]) && is_numeric($_GET["page"]))
    $currentPage = $_GET["page"];
$res = SQL("SELECT COUNT(*) FROM news_posts");
$itemTotal = $res[0]["COUNT(*)"];
$lastPage = intval($itemTotal / $itemsPerPage);
$skipItems = $currentPage * $itemsPerPage;
if ($lastPage < $currentPage)
    die("Invalid request");
$items = SQL("SELECT title, author, date, content FROM news_posts
                    ORDER BY date DESC LIMIT ? OFFSET ?", $itemsPerPage, $skipItems);
if ($items == null)
    dieDb($mysqli);
?>
<!DOCTYPE html>
<html>
<head>
<?php include "php/head.php"; ?>
</head>
<body>
<?php include "php/header.php"; ?>
<div id="main">
    <h2>Croupier poker AI</h2>
    <?php
    foreach ($items as $item) {
        echo "<p><h2>";
        echo $item["title"] . "</h2>";
        echo $item["author"] . "<br />";
        echo $item["date"] . "<br />";
        echo $item["content"];
        echo "</p>";
    }
    echo '<div id="newsNavigator">';
    if ($currentPage != 0)
        echo '<a class="newsNavigatorButton" href="' . $_SERVER["PHP_SELF"] . '?page=' . ($currentPage - 1) .
            '">' . $tr["NEWER_PAGES"] . '</a>';
    if ($currentPage != $lastPage)
        echo '<a class="newsNavigatorButton" style="float:right;" href="' . $_SERVER["PHP_SELF"] . '?page=' .
            ($currentPage + 1) . '">' . $tr["OLDER_PAGES"] . '</a>';
    echo '</div>';
    ?>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>