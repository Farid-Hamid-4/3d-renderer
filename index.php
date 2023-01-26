<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>WebGL Demo</title>
    <link rel="stylesheet" href="./webgl.css" type="text/css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js"
      integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ=="
      crossorigin="anonymous" defer>
    </script>
    <script src="webgl-demo.js" defer></script>
    <script src="loaddata.js" ></script>
  </head>

  <body>
  <div>
    <button type="button" onclick="window.open(); self.close();"> Exit </button>
    
      <FORM method="post" action="upload.php" enctype="multipart/form-data">
        Select File: <input type="file" name="uploadfilename" id="uploadfilename">
        <input type="submit" value="Upload">
      </FORM>
  </div>

  <script>
      function displayUpload(){
        s1 = document.getElementById('fileSelectButton').value;
        idx = s1.lastIndexOf("\\");
        s1 = s1.slice(idx+1)
        s = "Uploaded File: " + s1;
        alert(s);
      }
    </script>
  </body>
</html>
