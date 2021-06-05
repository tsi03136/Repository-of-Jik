<?php
    $url = 'http://172.30.1.4/';
    $ch = curl_init();
    
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    $res = curl_exec($ch);
    curl_close($ch);

    echo $res;
?>