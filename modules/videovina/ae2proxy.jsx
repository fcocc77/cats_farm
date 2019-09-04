//@include "utils.jsx"
//@include "aeUtils.jsx"

// resolucion para el proxy
var width = 640;
var height = 360;

var scale = (width * 100) / 1920;
var scalePlus = 1920 / width;
// ------------------------------

// ya que se crearan nuevos item en el proyecto antes
// de hacer todo crea una lista de las composiciones actuales
// para usarla en las funciones de abajo
var comps = getAllComps();
// -------------------------------------------

// Cambia la estala de la capa dependiendo de la resolucion del proxy,
// si la capa tiene key frames, reescala por cada key 
function changeScale(layer) {
	var scale = layer.scale;
	if (scale.numKeys == 0) {
		var newValue = scale.value * scalePlus;
		scale.setValue(newValue);
	}
	else {
		// Cambia escala por key frames
		for (var i = 1; i <= scale.numKeys; i++) {
			var value = scale.keyValue(i);
			var newValue = value * scalePlus;
			scale.setValueAtKey(i, newValue);
		}
	}
	//-----------------------------
}
// --------------------------------------

// Para encontrar una composicion en las layer es necesario que tengan
// el nombre original de la comp, asi esta funcion deja sin nombre todas
// las capas en todas las comp
function allLayerWithoutName() {
	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		for (var a = 1; a <= comp.layers.length; a++) {
			var layer = comp.layers[a];
			layer.locked = false;
			layer.name = "";
		}
	}
}
allLayerWithoutName();
// ---------------------------------------------

// Crea un null y vincula todas las capas que no esten enparentadas
// y luego reescala el null a la resolucion del proxy
function nullCreate(item) {
	var _null = item.layers.addNull();
	_null.position.setValue([0, 0]);

	for (var i = 1; i <= item.layers.length; i++) {
		var layer = item.layers[i];
		layer.locked = false;
		if (layer.parent == null)
			if (layer != _null)
				layer.parent = _null;
	}

	_null.scale.setValue([scale, scale]);
}
// --------------------------------------

// Cambia la resolucion en la comp y en el layer de la misma comp
function toProxy() {
	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		comp.width = width;
		comp.height = height;
		nullCreate(comp);
		// usedIn busca en que composicion esta, esta comp como capa
		for (var a = 0; a < comp.usedIn.length; a++) {
			var parentComp = comp.usedIn[a];
			var layer = parentComp.layer(comp.name);
			changeScale(layer);
		}
		// -----------------------------------
	}
}
// ---------------------------

toProxy();