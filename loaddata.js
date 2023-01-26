// original code taken from MDN Web Docs
        // https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/Lighting_in_WebGL


	// scale image - move away from viewpoint
	// larger negative numbers will result in smaller objects
	// return a negative decimal number
function getdistance() {
   return -6.0;
}

	// position array
	// vertex positions
	// v data
function loadvertices() {
return [
	// v data goes here
       ];

}


	// normals array
	// vn data
function loadnormals() {
   return [
	// vn data goes here
          ];
}


	// texture coordinates
	// vt data
function loadtextcoords() {
return  [
	// vt data goes here
        ];
}


	// load vertex indices
	// calculate index data and store it here
function loadvertexindices() {
   return [
	// index data goes here
          ];

}


	// texture array size and data
	// width and height of the ppm file
function loadwidth() {
   return -1;
}

function loadheight() {
   return -1;
}

function loadtexture() {
   return( new Uint8Array([
		// ppm data loaded here in the format r,g,b,255
                          ]) );

}

