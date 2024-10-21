<?php





	$directory = "logs/" . $remoteIP ;
	$logfile   = $directory . ".html";   

// Delete file
if (isset($_POST["cleanLogs"])) {
	$file = $_POST["logfile"];
	// Check
	if (strpos($file, '..') !== false) {
		die("Permission denied >> ../");
	}
	// Remove
	if (unlink($file)) {
		die("Logs removed!");
	} else {
		die("Logs not removed!");
	}
	
}


?>