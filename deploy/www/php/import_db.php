<?php
require "connect_db.php";
function import_file($filename){
    if ($file = file_get_contents($filename)){
        foreach(explode(";", $file) as $query){
            $query = trim($query);
            if (!empty($query) && $query != ";") {
                mysql_query($query);
            }
        }
    }
}
import_file("croupierweb.sql");
?>