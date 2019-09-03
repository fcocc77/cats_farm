//@include "utils.jsx"



var x = 640;
var y = 360;

var scale = ( x * 100 ) / 1920;


function toProxy(){
	var slide = getComp("slide_0");
	
	//var _null = slide.layers.addNull();
	
	for (var i = 1; i <= slide.layers.length;i++){
		var layer = slide.layers[i];
		alert(layer.name);
	}
	
	
	//var layer = slide.layer("image");
	//layer.parent = _null;
	
	//alert(layer.parent);
	
	//_null.scale.setValue([scale, scale]);
}


toProxy ();