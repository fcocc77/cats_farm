//@include "utils.jsx"
//@include "aeUtils.jsx"

/*****************************************************************************
 * proxyExport.jsx: Script After Efects
 *****************************************************************************
 * 2018-2019 VideoVina, Santiago de Chile
 *
 * Authors: Francisco Contreras Cuevas <fcocc77@gmail.com>
 *
 * Este script agrega a la cola de render todos los archivos que 
 * son secuencia de imagenes y las deja en proxy a un tercio de su resolucion,
 * es necesario que este creada la plantilla en after effect 
 * llamada proxy para los modulos de render y salida.
 *****************************************************************************/

function proxyExport() {
	var items = getFileItems();
	for (var i = 0; i < items.length; i++) {

		var item = items[i].item;
		var src = items[i].src;
		var file = String(src.file);

		if (isSequence(src)) {
			var _file = filePathBreak(file);
			var proxyFile = _file.base + "_proxy_[" + _file.padding + "]." + _file.ext;
			var render = app.project.renderQueue.items.add(item);

			// cambia la plantilla
			render.applyTemplate("proxy");
			render.outputModules[1].applyTemplate("proxy");
			// ----------------------------------------

			// Cambia el nombre de la salida
			render.outputModule(1).file = new File(proxyFile);
			// ------------------------------------
		}
	}
}


proxyExport();