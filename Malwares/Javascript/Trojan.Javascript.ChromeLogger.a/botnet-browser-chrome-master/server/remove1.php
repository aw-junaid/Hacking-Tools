<?php


 // remove all logs (dashboard)

if(isset($_POST['cleanLogs']))
{
    $file_Path = $_POST['log_file'];
    
    // check if the file exist
    if(file_exists($file_Path))
    {
        unlink($file_Path);
       // echo 'File Deleted';
     header('location:index.php');
    }else{
        echo 'File Not Exist';
    }
}


?>