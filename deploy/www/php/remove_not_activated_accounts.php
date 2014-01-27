<?php
$deltaDay = 5;
$date = new DateTime();
$date->sub(new DateInterval('P' . $deltaDay . 'D'));
SQL("DELETE FROM accounts a, account_activation aa WHERE a.id == aa.id AND aa.sendTime < ?", $date->format("Y-m-d H:i:s"));