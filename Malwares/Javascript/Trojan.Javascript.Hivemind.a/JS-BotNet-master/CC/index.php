<?php
                                    if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
                  $ip = $_SERVER['HTTP_CLIENT_IP'];
                } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
            $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
                } else {
            $ip = $_SERVER['REMOTE_ADDR'];
                }
            $space = "\n"; 
            date_default_timezone_set('America/New_York');
            $date = date('l jS \of F Y h:i A');
            $browser = $_SERVER['HTTP_USER_AGENT'];
            $data = "Bot Browser Data = ";
            $browserdata = $data . $browser;
            $fullip = 'Bot IPv4 Address = ' . $ip ;
            $track = 'Track Bot Location = https://www.ip-tracker.org/locator/ip-lookup.php?ip=' . $ip ;
                        
                            function ip_visitor_country()
                {
                
                    $client  = @$_SERVER['HTTP_CLIENT_IP'];
                    $forward = @$_SERVER['HTTP_X_FORWARDED_FOR'];
                    $remote  = $_SERVER['REMOTE_ADDR'];
                    $country  = "Unknown";
                
                    if(filter_var($client, FILTER_VALIDATE_IP))
                    {
                        $ip = $client;
                    }
                    elseif(filter_var($forward, FILTER_VALIDATE_IP))
                    {
                        $ip = $forward;
                    }
                    else
                    {
                        $ip = $remote;
                    }
                    $ch = curl_init();
                    curl_setopt($ch, CURLOPT_URL, "http://www.geoplugin.net/json.gp?ip=".$ip);
                    curl_setopt($ch, CURLOPT_HEADER, 0);
                    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
                    $ip_data_in = curl_exec($ch); // string
                    curl_close($ch);
                
                    $ip_data = json_decode($ip_data_in,true);
                    $ip_data = str_replace('&quot;', '"', $ip_data); // for PHP 5.2 see stackoverflow.com/questions/3110487/
                
                    if($ip_data && $ip_data['geoplugin_countryName'] != null) {
                        $country = $ip_data['geoplugin_countryName'];
                    }
                
                    return 'Bot IPv4 Address = '.$ip.' # Country: '.$country;
                }
                

           
  

      
            
            file_put_contents('log.txt', $date, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $space, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', ip_visitor_country(), FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $space, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $browserdata, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $space, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $track, FILE_APPEND | LOCK_EX);            
            file_put_contents('log.txt', $space, FILE_APPEND | LOCK_EX);
            file_put_contents('log.txt', $space, FILE_APPEND | LOCK_EX);
            
            
            
            
            
 
            
?>
