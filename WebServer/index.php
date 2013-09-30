<?php
include "php/include.php";
$currentPage = 0;
$itemsPerPage = 10;
if (isset($_GET["page"]) && !empty($_GET["page"]) && is_numeric($_GET["page"]))
    $currentPage = $_GET["page"];
if ($result = $mysqli->query("SELECT COUNT(*) FROM news_posts")) {
    $itemTotal = $result->fetch_row()[0];
    $result->free();
} else
    dieDb($mysqli);
$lastPage = intval($itemTotal / $itemsPerPage);
$skipItems = $currentPage * $itemsPerPage;
if ($lastPage < $currentPage)
    die("Invalid request");
if ($stmt = $mysqli->prepare("SELECT title, author, date, contents
    FROM news_posts ORDER BY date DESC LIMIT ? OFFSET ?")
) {
    $stmt->bind_param('ii', $itemsPerPage, $skipItems);
    $stmt->execute();
    $result = $stmt->get_result();
    $items = $result->fetch_all(MYSQLI_ASSOC);
    $stmt->close();
} else dieDb($mysqli);
?>
<!DOCTYPE html>
<html>
<?php include "php/head.php"; ?>
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
        echo $item["contents"];
        echo "</p>";
    }
    echo '<a href="' . $_SERVER["PHP_SELF"] . '?page=0">First page</a>';
    for ($i = 1; $i < $lastPage; $i++) {
        echo '<a href="' . $_SERVER["PHP_SELF"] . '?page=' . $i . '">' . $i . '</a>';
    }
    echo '<a href="' . $_SERVER["PHP_SELF"] . '?page=' . $lastPage . '">Last page</a>';
    ?>
</div>
<div id="footer"></div>
</body>
</html>