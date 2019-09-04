//@include "utils.jsx"

var width = 640;
var height = 360;

var scale = (width * 100) / 1920;
var scalePlus = 1920 / width;


function toProxy(item) {
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

	//_null.remove();
}

function changeScale(layer) {
	// Cambia escala por key frames
	var scale = layer.scale;

	if (scale.numKeys = 0) {
		var newValue = scale.value * scalePlus;
		scale.setValue(newValue);
		return;
	}

	for (var i = 1; i <= scale.numKeys; i++) {
		var value = scale.keyValue(i);
		var newValue = value * scalePlus;
		scale.setValueAtKey(i, newValue);
	}
	//-----------------------------
}

function getAllComp() {
	var comps = [];
	for (var i = 1; i <= app.project.numItems; i++) {
		var item = app.project.item(i);
		if (item instanceof CompItem)
			comps.push(item);
	}
	return comps;
}
var comps = getAllComp();

function changeSize() {
	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		comp.width = width;
		comp.height = height;
		toProxy(comp);
		for (var a = 0; a < comp.usedIn.length; a++) {
			var parentComp = comp.usedIn[a];
			var layer = parentComp.layer(comp.name);
			changeScale(layer);
		}
	}
}

function allWithoutName() {
	for (var i = 0; i < comps.length; i++) {
		var comp = comps[i];
		for (var a = 1; a <= comp.layers.length; a++) {
			var layer = comp.layers[a];
			layer.name = "";
		}
	}
}

//changeSize();
allWithoutName();