<?php
    $url = 'http://192.168.31.129/';
    $ch = curl_init();
    
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    $res = curl_exec($ch);
    curl_close($ch);

    echo $res;
?>