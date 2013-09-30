<div id="header">
    <div id="topmenu">
        <ul>
            <?php
            echo '<li><a href="./">' . $tr["MAIN"] . '</a></li>';
            if ($loggedin) {
                echo '<li><a href="summary.php">' . $tr["SUMMARY"] . '</a></li>';
                echo '<li><a href="manage_bots.php">' . $tr["MANAGE_BOTS"] . '</a></li>';
                echo '<li><a href="leaderboards.php">' . $tr["LEADERBOARDS"] . '</a></li>';
            } else {
                echo '<li><a href="login.php">' . $tr["LOGIN"] . '</a></li>';
                echo '<li><a href="register.php">' . $tr["REGISTER"] . '</a></li>';
            } ?>
        </ul>
    </div>
    <div id="lang">
        <a href="../setlang.php?lang=en">EN</a> |
        <a href="../setlang.php?lang=hu">HU</a>
    </div>
    <?php
    if ($loggedin) {
        echo '<div id="accountMenu">';
        echo '<a id="username_accountMenu">' . $_SESSION["username"] . '</a>';
        echo '<a id="logout_accountMenu" href="logout.php">' . $tr["LOGOUT"]. '</a>';
        echo '</div>';
    }
    ?>
</div>