<?php
function getBotInfos($mysqli)
{
    if ($stmt = $mysqli->prepare("SELECT id, name, add_time, code_lang, state
    FROM bots WHERE account_id = ?")
    ) {
        $stmt->bind_param('s', $_SESSION["account_id"]);
        $stmt->execute();
        $result = $stmt->get_result();
        $arr = $result->fetch_all(MYSQLI_ASSOC);
        $stmt->close();
        return $arr;
    }
    return array();
}