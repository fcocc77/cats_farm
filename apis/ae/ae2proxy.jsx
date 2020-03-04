//@include "utils.jsx"
//@include "aeUtils.jsx"

/*****************************************************************************
 * ae2proxy.jsx: Script After Efects
 *****************************************************************************
 * 2018-2019 VideoVina, Santiago de Chile
 *
 * Authors: Francisco Contreras Cuevas <fcocc77@gmail.com>
 *
 * Este script reescala todas las composiciones y footage a una resolucion mas baja (proxy)
 * antes de ejecutar este script es necesario crear el proxy de todas las sequencias de imagenes
 * esto se hace con el script "proxyExport.jsx"
 *****************************************************************************/

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

function changeScale(layer) {
	// Cambia la estala de la capa dependiendo de la resolucion del proxy,
	// si la capa tiene key frames, reescala por cada key 
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

function allLayerWithoutName() {
	// Para encontrar una composicion en las layer es necesario que tengan
	// el nombre original de la comp, asi esta funcion deja sin nombre todas
	// las capas en todas las comp

	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		for (var a = 1; a <= comp.layers.length; a++) {
			var layer = comp.layers[a];
			layer.locked = false;
			layer.name = "";
		}
	}
}

function nullCreate(item) {
	// Crea un null y vincula todas las capas
	// y luego reescala el null a la resolucion del proxy
	var _null = item.layers.addNull();
	_null.position.setValue([0, 0]);

	for (var i = 1; i <= item.layers.length; i++) {
		var layer = item.layers[i];
		if (layer.name != _null.name)
			layer.parent = _null;
	}
	_null.scale.setValue([scale, scale]);
	_null.remove();
}

function proxysImport() {
	// Remplaza el material de las secuencias por el proxy
	var items = getFileItems();

	for (var i = 0; i < items.length; i++) {
		var item = items[i].item;
		var type = items[i].type;
		var file = String(items[i].src.file);

		var _file = filePathBreak(file);
		var proxyFile = _file.base + "_proxy_" + _file.number + "." + _file.ext;

		if (isFile(proxyFile)) {
			if (type == "file")
				item.replaceWithSequence(File(proxyFile), null);
			if (type == "proxy")
				item.setProxyWithSequence(File(proxyFile), null);
		}
		// -------------------------------------------
	}
}

function unParentAll() {
	// esta funcion borra el parent de todas las layer en todas las comp y
	// las guarda en una lista para despues restaurarlas

	var compsLayers = [];
	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];

		//el indicador de tiempo tiene que estar en la mitad
		//para que sea correcto el unparent
		comp.time = comp.duration / 2
		//------------------------

		// lista para layer emparentadas, para revertir el cambio al final
		var layersWithParent = [];
		// -----------------------------------

		for (var e = 1; e <= comp.layers.length; e++) {
			var layer = comp.layers[e];
			layer.locked = false;
			// si la capa esta emparentada la agrega a la lista
			if (layer.parent != null)
				layersWithParent.push({ "index": layer.index, "parent": layer.parent });
			// ---------------------------------------------------------
			layer.parent = null;
		}
		compsLayers.push({ "comp": comp, "layers": layersWithParent });
	}

	return compsLayers;
}

function parentAll(compsLayers) {
	for (var i = 0; i < compsLayers.length; i++) {
		var comp = compsLayers[i].comp;
		var layersWithParent = compsLayers[i].layers;

		//el indicador de tiempo tiene que estar en la mitad
		//para que sea correcto el parent
		comp.time = comp.duration / 2
		//------------------------

		for (var e = 0; e < layersWithParent.length; e++) {
			var layer = layersWithParent[e];
			comp.layers[layer.index].parent = layer.parent;
		}
	}
}

function main() {
	// Cambia la resolucion en la comp y en el layer de la misma comp
	allLayerWithoutName();
	// es necesario que esten ya los archivos proxy ( se crean con proxyExport.jsx )
	proxysImport();
	// --------------------------

	// desenparenta todas las capas para que no tenga problema el reescalado
	var compsLayers = unParentAll();
	// ----------------------------------------

	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		comp.width = width;
		comp.height = height;
		nullCreate(comp);
		// usedIn busca en que composicion esta, esta comp como capa
		for (var a = 0; a < comp.usedIn.length; a++) {
			var parentComp = comp.usedIn[a];
			var layers = layersByName(parentComp, comp.name);

			for (var e = 0; e < layers.length; e++)
				changeScale(layers[e]);
		}
		// -----------------------------------
	}

	// vuelve a conectar todos los parent como antes
	parentAll(compsLayers);
	// --------------------------------------------
}

main();