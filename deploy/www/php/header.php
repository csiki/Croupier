<div id="header">
    <div id="topmenu">
        <ul>
            <?php
            echo '<li><a href="/">' . $tr["MAIN"] . '</a></li>';
            if ($loggedin) {
                echo '<li><a href="summary.php">' . $tr["SUMMARY"] . '</a></li>';
                echo '<li><a href="my_bots.php">' . $tr["MY_BOTS"] . '</a></li>';
                echo '<li><a href="leaderboards.php">' . $tr["LEADERBOARDS"] . '</a></li>';
                echo '<li><a href="help.php">' . $tr["HELP"] . '</a></li>';
                if ($admin)
                    echo '<li><a href="admin.php">Admin</a></li>';
            } else {
                echo '<li><a href="login.php">' . $tr["LOGIN"] . '</a></li>';
                echo '<li><a href="register.php">' . $tr["REGISTER"] . '</a></li>';
            } ?>
        </ul>
        <script>
            $("#topmenu ul li").on("click", function () {
                window.location.href = $(this).children().attr("href");
            });
        </script>
    </div>
    <div id="lang">
        <a href="../setlang.php?lang=en">EN</a> |
        <a href="../setlang.php?lang=hu">HU</a>
    </div>
    <?php
    if ($loggedin) {
        echo '<div id="accountMenu">';
        echo '<a id="username_accountMenu" href="edit_account.php">';
        echo '<img src="https://www.gravatar.com/avatar/' .
            $_SESSION["gravatar"] . '?d=identicon&s=25&r=g" />';
        echo $_SESSION["username"];
        echo '</a>';
        echo '<ul class="dropDownMenu">';
        echo '<li><a href="edit_account.php">' . $tr["EDIT_ACCOUNT"] . '</a></li>';
        echo '<li><a href="logout.php">' . $tr["LOGOUT"] . '</a></li>';
        echo '</ul>';
        echo '</div>';
    }
    ?>
</div>
<div id="headerTitle">Croupier poker framework</div>