//@include "external/json2.js"

// Obtiene ruta de proyecto
var path = app.project.file.toString();
path = path.substring(0, path.lastIndexOf('/'));
//----------------------	

function fread(_file) {
    var file = File(_file);
    file.open("r");
    var data = file.read();
    file.close();
    return data;
}

function fwrite(file, data) {
    var file = File(file);
    file.open("w");
    file.write(data);
    file.close();
}

function jread(_file) {
    // Obtiene archivo json string y lo evalua para convertirlo en objeto
    eval("var data =" + fread(_file));
    // -------------------------------
    return data;
}

function jwrite(file, data) {
    // Convierte texto unicode a string normal
    function unicodeToChar(text) {
        return text.replace(/\\u[\dA-F]{4}/gi,
            function (match) {
                return String.fromCharCode(parseInt(match.replace(/\\u/g, ''), 16));
            });
    }
    // ---------------------------------------------

    var jsonString = unicodeToChar(JSON.stringify(data));
    fwrite(file, jsonString);
}

function isDir(_path) {
    var folder = new Folder(_path);
    if (folder.exists)
        return true;
    else
        return false;
}

function isFile(_path) {
    var file = new File(_path);

    if (file.exists)
        return true;
    else
        return false;
}

function basename(_path) {
    return String(_path).split("/").pop();
}

function dirname(_path) {
    _path = String(_path);
    return _path.substring(0, _path.lastIndexOf('/'));
}