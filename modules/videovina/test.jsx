//@include "external/json2.js"

// Este script obtiene informacion externa a partir de un json creado por la aplicacion,
// y modifica textos, colores y tiempos de la plantilla after effect
{
    var projectFile = new File("C:/Users/Pancho/Documents/GitHub/videovina/slideshows/2sa5f8/videovina.aep");
    app.open(projectFile);

    // Obtiene ruta de proyecto
    var path = app.project.file.toString();
    path = path.substring(0, path.lastIndexOf('/'));
    //----------------------
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
    var comp = getComp('Comp 1');
    var text = comp.layer(1);
    var solid = comp.layer(2);

    // Obtiene archivo de informacion
    var file = File(path + "/project.json");
    file.open("r");
    var project = JSON.parse(file.read());
    file.close();
    // -------------------------------

    // Extrae los colores del string rgb() de css
    var colors = project.color.replace("rgb(", "").replace(")", "").split(",");
    var red = parseInt(colors[0]);
    var green = parseInt(colors[1]);
    var blue = parseInt(colors[2]);
    var color = [red, green, blue] / 255;
    //-------------------------------

    solid.effect("Fill").color.setValue(color);
    text.property("Source Text").setValue(project.title);

    // Cambia el tiempo de la composicion
    var originalTime = 10;
    var stretch = (project.time * 100) / originalTime;

    var compParent = getComp('final').layer(1);
    compParent.stretch = stretch;
    //--------------------------------------

    app.project.save();
}