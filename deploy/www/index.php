<?php
require_once "php/include.php";
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
    <?php require "php/head.php"; ?>
</head>
<body>
<?php require "php/header.php"; ?>
<div id="main">
    <h2><?= $tr["WEBPAGENAME"] ?></h2>

    <div style="margin: 0 auto; width: 700px; padding: 5px; margin-bottom: 20px;">
        <p style="text-align: justify">
            Üdvözöllek a Croupier oldalán, ahol Texas Hold'em mesterséges intelligenciák mérhetik össze tudásukat! A
            Croupier projekt azért jött létre, hogy egy egyszerű környezetet biztosítson poker bot íráshoz, így többen
            beleáshatják magukat a mesterséges intelligencia fejlődő világába.
            A regisztrációt követően, a <a style="color: #000000;" href="docs/mi_dev/miDev.html">manuálból<a/>
                kimásolhatsz a példabotot és el
                is indíthatod a versenyen. Logokon keresztül figyelheted, hogy teljesít, hogy reagál a többi játékos
                lépéseire. A botodat folyamatosan fejlesztheted és nyomonkövetheted eredményeit, amit
                percenként frissítünk.
        </p>
        <?php
        if (!$loggedin)
        {
        ?>
        <div style="text-align: center">
            <button class="mainRegButton" type="button"
                    onclick="window.location.href = 'register.php';"><?= $tr["REGISTER"] ?></button>
        </div>

        <?php } ?>
    </div>
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
    <?php require "php/footer.php"; ?>
</footer>
</body>
</html>