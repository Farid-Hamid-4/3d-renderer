<?php
if ($_FILES) {
	shell_exec("export PYTHONPATH=\"{PYTHONPATH}:./\"");
	$name = $_FILES["uploadfilename"]["name"];
	$name = substr($name, 0, strrpos($name, "."));
	echo ('Uploaded ' .  $name);
	$cmd = "./a1 ".$name;
	echo($cmd);
	exec($cmd, $output, $status);
	if($status){
		echo("call failed\n");
	}else{
		$html = "	<!doctype html>
		<html lang=\"en\">
		<head>
		<meta charset=\"utf-8\">
		<title>WebGL Demo</title>
		<link rel=\"stylesheet\" href=\"./webgl.css\" type=\"text/css\">
		<script src=\"https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js\"integrity=\"sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ==\"crossorigin=\"anonymous\" defer></script>
		<script src=\"webgl-demo.js\" defer></script>
	    <script src=\"loaddata.js\" ></script>
		</head>
		<body>
		<canvas id=\"glcanvas\" width=\"640\" height=\"480\"></canvas>
		</body>
		</html>";
		echo($html);
	}
	//move_uploaded_file($_FILES["uploadfilename"]["tmp_name"], $name);
}
?>
