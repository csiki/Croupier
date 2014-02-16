<?php
require "connect_db.php";
require "functions.php";


SQL("UPDATE games SET checked = 1, endTime = NOW() WHERE id = ?", $id); // from gameresult