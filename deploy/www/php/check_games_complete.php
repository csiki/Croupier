<?php
include "connect_db.php";
include "functions.php";


SQL("UPDATE games SET checked = 1, endTime = NOW() WHERE id = ?", $id); // from gameresult