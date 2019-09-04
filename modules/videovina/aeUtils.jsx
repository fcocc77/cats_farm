//@include "utils.jsx"
function getComp(name) {
    var myComp;
    for (var i = 1; i <= app.project.numItems; i++) {
        if ((app.project.item(i) instanceof CompItem) && (app.project.item(i).name === name)) {
            myComp = app.project.item(i);
            break;
        }
    }
    return myComp;
}

function getAllComps() {
    var comps = [];
    for (var i = 1; i <= app.project.numItems; i++) {
        var item = app.project.item(i);
        if (item instanceof CompItem)
            comps.push(item);
    }
    return comps;
}

function getItem(name) {
    for (var i = 1; i <= app.project.numItems; i++) {
        if (app.project.item(i).name == name) {
            return app.project.item(i);
        }
    }
    return false;
}

function getAllItems() {
    var items = [];
    for (var i = 1; i <= app.project.numItems; i++) {
        var item = app.project.item(i);
        items.push(item);
    }
    return items;
}

function getFileItems() {
    // esta funcion obtiene todos los item que tengan una ruta de 
    // archivo en file o en el proxy
    var items = getAllItems();
    var itemsFiles = [];
    for (var i = 0; i < items.length; i++) {
        var item = items[i];

        // checkea si el item tiene file o proxy y los guarda con un numero 1 es file y 2 es proxy
        if (item.file != undefined)
            itemsFiles.push({ "type": "file", "item": item, "src": item.mainSource });
        if (item.proxySource != undefined && item.proxySource != null)
            itemsFiles.push({ "type": "proxy", "item": item, "src": item.proxySource });
        // ------------------------------------
    }

    return itemsFiles;
}

function isSequence(src) {
    var exts = ["jpg", "png", "JPG", "PNG"];
    file = String(src.file);
    var ext = file.split(".").pop();
    // si el archivos tiene las extenciones de arriba y no es un still, es una secuencia
    for (var i = 0; i < exts.length; i++) {
        if (ext == exts[i])
            if (!src.isStill)
                return true;
    }
    // ------------------------------

    return false;
}

function filePathBreak(file) {
    var basePath = file.split('_').slice(0, -1).join('_');
    var number = file.split("_").pop().split(".")[0];
    var ext = file.split(".").pop();
    //crea # para la sequencia dependiendo de la cantidas de caracteres del padding
    var padding = "";
    for (var i = 0; i < number.length; i++)
        padding += "#";
    // ---------------------------------------------

    return { "base": basePath, "ext": ext, "padding": padding, "number": number };
}