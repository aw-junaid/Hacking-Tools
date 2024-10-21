<?php
require_once('geoplugin.class.php');
$geoplugin = new geoPlugin();
// If we wanted to change the base currency, we would uncomment the following line
// $geoplugin->currency = 'EUR';
 
$geoplugin->locate();
 
echo "Geolocation results for {$geoplugin->ip}: <br />\n".
	"City: {$geoplugin->city} <br />\n".
		"<img src='http://api.hostip.info/flag.php?ip=$ip' /> <br />\n".
		
	"Region: {$geoplugin->region} <br />\n".
	"Region Code: {$geoplugin->regionCode} <br />\n".
	"Region Name: {$geoplugin->regionName} <br />\n".
	"DMA Code: {$geoplugin->dmaCode} <br />\n".
	"Country Name: {$geoplugin->countryName} <br />\n".
	"Country Code: {$geoplugin->countryCode} <br />\n".
	"In the EU?: {$geoplugin->inEU} <br />\n".
	"EU VAT Rate: {$geoplugin->euVATrate} <br />\n".
	"Latitude: {$geoplugin->latitude} <br />\n".
	"Longitude: {$geoplugin->longitude} <br />\n".
	"Radius of Accuracy (Miles): {$geoplugin->locationAccuracyRadius} <br />\n".
	"Timezone: {$geoplugin->timezone}  <br />\n".
	"Currency Code: {$geoplugin->currencyCode} <br />\n".
	"Currency Symbol: {$geoplugin->currencySymbol} <br />\n".
	"Exchange Rate: {$geoplugin->currencyConverter} <br />\n";
 
/* find places nearby */
if ( isset($nearby[0]['geoplugin_place']) ) {
	echo "<pre><p>Some places you may wish to visit near " . $geoplugin->city . ": </p>\n";
	foreach ( $nearby as $key => $array ) {
 
		echo ($key + 1) .":<br />";
		echo "\t Place: " . $array['geoplugin_place'] . "<br />";
		echo "\t Country Code: " . $array['geoplugin_countryCode'] . "<br />";
		echo "\t Region: " . $array['geoplugin_region'] . "<br />";
		echo "\t County: " . $array['geoplugin_county'] . "<br />";
		echo "\t Latitude: " . $array['geoplugin_latitude'] . "<br />";
		echo "\t Longitude: " . $array['geoplugin_longitude'] . "<br />";
		echo "\t Distance (miles): " . $array['geoplugin_distanceMiles'] . "<br />";
		echo "\t Distance (km): " . $array['geoplugin_distanceKilometers'] . "<br />";
 
	}
	echo "</pre>\n";
}
?>
