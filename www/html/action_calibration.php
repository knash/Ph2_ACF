<html>
<body>

<form action="stop.php" method=post>
<input type="submit" name="ciao" value="STOP">
</form>
<?php
ob_implicit_flush(true);
ob_end_flush();
set_include_path(get_include_path() . PATH_SEPARATOR . 'amassett/Ph2_ACF/www/html');
error_reporting(E_ALL);
ini_set('display_errors', 1);
 
$current_path=shell_exec("pwd");
$command=null;
$formatted="calibrate";
$file=fopen("initialize.sh","w") or die("Unable to open file!");
$calibr_type = $_POST['type'];
if($calibr_type=="old")
$formatted=$formatted." --old";

if(isset($_POST['ScanVPlus']))
	$formatted=$formatted." --skip";

if(isset($_POST['Bitwise']))
$formatted=$formatted." --bm";

if(isset($_POST['All_channel']))
$formatted=$formatted." -a";

$formatted=$formatted." -o ".$_POST['output'];

$formatted=$formatted." -f ".$_POST['Hw_Description_File_'];


echo $formatted;

echo "<br/>";

fwrite($file, "#!/bin/bash"."\n"."cd ../../"."\n"."source "."$(pwd)/setup.sh" ."\n".$formatted);
fclose($file);


$command =  preg_replace('/\s+/', '', $current_path.'/initialize.sh');
$handle = popen("source ".$command, "r");
$continue=TRUE;
$count=10;
while ((!feof($handle))&& $continue) {
    $data = fgets($handle);
    print $data."<br/>";
    echo $continue;

       if($count>65)
         $continue=FALSE;
     echo $count;
     $count++;
      
  	// if(isset($_POST['exec1'])==TRUE){
  	// BREAK;
   // }
   
}
shell_exec("kill $(pidof calibrate)");
exit();

// $command =  preg_replace('/\s+/', '', $current_path.'/initialize.sh');
//  exec("source ".$command, $output);
//   foreach ( $output as $item ) {
//         echo $item . "<br/>";
//     }

?>

</body>
</html>  
