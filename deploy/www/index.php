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

    <div class="postsContainer">
        <?php
        echo '<div id="postSection">';
        foreach ($items as $item) {
            echo '<div class="post">';
            echo "<h2>" . $item["title"] . "</h2>";
            echo '<div class="postHeader">' . $item["author"] . " - " . $item["date"] . "</div>";
            echo '<div class="postContent">' . $item["content"] . '</div>';
            echo '</div>';
        }
        echo '</div>';
        if ($itemTotal > $itemsPerPage) {
            echo '<div id="postsNavigator">';
            if ($currentPage != 0)
                echo '<a class="postsNavigatorButton" href="' . $_SERVER["PHP_SELF"] . '?page=' . ($currentPage - 1) .
                    '">' . $tr["NEWER_PAGES"] . '</a>';
            if ($currentPage != $lastPage)
                echo '<a class="postsNavigatorButton" style="right:0px;" href="' . $_SERVER["PHP_SELF"] . '?page=' .
                    ($currentPage + 1) . '">' . $tr["OLDER_PAGES"] . '</a>';
            echo '</div>';
        }
        ?>
    </div>
</div>
<footer>
    <?php include "php/footer.php"; ?>
</footer>
</body>
</html>