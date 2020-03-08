﻿/*****************************************************************************
 * modes.jsx: Script After Efects
 *****************************************************************************
 * 2018-2019 VideoVina, Santiago de Chile
 *
 * Authors: Francisco Contreras Cuevas <fcocc77@gmail.com>
 *
 * Este script actualiza diferentes atributos de todas las slides que 
 * estan ubicadas en la capa "control" de la composicion "final", y esta modifica
 * textos, fotos, modos de fucion y colores
 *****************************************************************************/

// La variable aep de define el vinarender, si no se usa vinarender para este script lo omite
try {
    var projectFile = new File(aep);
    app.open(projectFile);
}
catch (error) { }
// --------------------------------

//@include "utils.jsx"
//@include "aeUtils.jsx"

// ----------------------------
var project = jread(path + "/project.json");
// ----------------------------

var finalComp = getComp("Final comp");
var slidesLayer = finalComp.layer("Slides");
var slidesComp = getComp("Slides");
var color = project.color.rgb / 255;

// declaracion de variables globales
var photos = [];
var endTime;
var endFrame;
var transition = 2; // Transicion en segundos
// ----------------------------


function main() {
    slidesLayer.effect("Color").color.setValue(color);

    photosSorted();
    timeModifier();
    songModifier();
    disableSlides();
    deleteMovies();

    for (var i = 0; i < photos.length; i++) {
        photosLoads(i);
        slideModifier(i);
        textModifier(i);
    }

    // actualiza las rutas de los assets
    updateFilesPath();
    // ----------------------------

    renderQueue(finalComp);

    // Guarda informacion en un json para poder obtenerla en el vinarender
    var submit = { "last_frame": endFrame };
    var submitJson = path + "/submit.json";
    jwrite(submitJson, submit);
    //-------------------------------------

    app.project.save();
}

function updateFilesPath() {
    var items = getFileItems();
    for (var i = 0; i < items.length; i++) {
        var item = items[i].item;
        var type = items[i].type;
        var src = String(items[i].src.file);

        // crea una ruta relativa haciendole un split con el nombre de la carpeta del proyecto "project.type"
        var relative = src.split(project.type).pop();
        relative = relative.replace("Assets", "assets"); // cambia assest si esta con mayuscula
        // --------------------------------------------

        // nueva ruta a partir del directorio donde estan los proyectos originals ( esta ruta la da vinarender )
        var footagePath = project.slideshowPath + "/" + project.type + "/" + relative;
        // --------------------------

        // si el directorio del archivo existe setea el file o proxy, se hace esto 
        // por hay archivos que estan dentro del la carpeta de proyecto del usuario 
        // que no necesitas cambiarse, solo es necesario en los assets del proyecto original
        if (isDir(dirname(footagePath))) {
            if (type == "file")
                item.replace(File(footagePath));
            if (type == "proxy")
                item.setProxyWithSequence(File(footagePath), null);
        }
        // -------------------------------------------
    }
}

function textModifier(index) {
    var textComp = getComp("Text " + index);
    if (textComp != undefined) {

        // Cambia Fuente a todos los textos
        var title = "Este es el Titulo";
        var subtitle = "Este es el subtitulo de titulo la de segunda linea";

        //en el subtitulo lo rompe en dos lineas	
        var subtitleNew = "";
        var words = subtitle.split(" ");
        var count = words.length - 1;
        var mid = Math.round(words.length / 2);
        for (var w = 0; w <= count; w++) {
            var word = words[w];
            if (w == mid)
                subtitleNew += word + "\r"; // quebre de linea en la mitad
            else if (w == count)
                subtitleNew += word; // sin espacio en el caracter final
            else
                subtitleNew += word + " "; // espacio por caracter
        }
        // --------------------------------------------------

        // borra las capas que no son texto
        for (var f = 1; f <= textComp.layers.length; f++) {
            var layer = textComp.layer(f);
            if (layer.text == undefined)
                layer.remove();
        }
        //--------------------------------------


        textComp.layer(1).property("Source Text").setValue(title);
        textComp.layer(2).property("Source Text").setValue(subtitleNew);

        for (var f = 1; f <= textComp.layers.length; f++) {

            var layer = textComp.layer(f);
            var textProp = layer.property("Source Text");
            var textDocument = textProp.value;
            textDocument.font = project.edit.font;
            textProp.setValue(textDocument);

            // ajusta texto al tamaño de la comp y lo centra
            var myRect = layer.sourceRectAtTime(0, true);
            var s = 100 * textComp.width / myRect.width;

            // El subtitulo es un poco mas chico que el titulo
            var subtitleMargin = 30;
            if (f == 2)
                s -= subtitleMargin;
            // ---------------------------

            layer.property("Scale").setValue([s, s]);

            // Cambia punto de anclaje
            var height = layer.sourceRectAtTime(textComp.time, false).height;
            var top = layer.sourceRectAtTime(textComp.time, false).top;
            var bottom = top + height;
            var margin = 5;


            if (f == 1) {
                layer.property("Anchor Point").setValue([0, bottom + margin]);
            }
            else {
                layer.property("Anchor Point").setValue([0, top - margin]);
            }
            // ----------------------------------------------

            layer.property("Position").setValue([(textComp.width / 2), (textComp.height / 2)]);
            // ------------------------------------------
        }
        // -------------------------------------
    }
}

function deleteMovies() {
    // Borra todos los archivos con extencion de video,
    // por que el after effect con wine da errores cuando hay
    // un video inportado, y es mas lento
    var deletes = [];
    for (var i = 1; i <= app.project.numItems; i++) {
        var item = app.project.item(i);
        if (item.file != undefined) {
            var ext = String(item.file).split(".").pop();
            if (ext == "mov" || ext == "mp4" || ext == "avi")
                deletes.push(item);
        }
    }

    for (var i = 0; i < deletes.length; i++) {
        deletes[i].remove();
    }
}

function songModifier() {
    // Importacion de musica
    var songName = project.song + ".mp3";
    //elimina la cancion si esta inportada
    if (getItem(songName))
        getItem(songName).remove();
    // -------------------------

    var io = new ImportOptions(File(project.songPath));
    var importedSong = app.project.importFile(io);

    finalComp.layers.add(importedSong);
    var songLayer = finalComp.layer(1);
    songLayer.startTime = 0; // la deja en el primer frame
    // baja el nivel de audio en el final
    var level = songLayer.property("Audio Levels");
    level.setValueAtTime(endTime - transition, [0, 0]);
    level.setValueAtTime(endTime, [-48, -48]);
    // ---------------------------------------------------
}

function photosSorted() {
    // crea la lista de fotos que estan en la linea de tiempo
    // y luego las ordena segun el index
    for (var name in project.media.pictures) {
        var photo = project.media.pictures[name];

        if (photo.timeline)
            photos.push({ "name": name, "index": photo.index })
    }

    photos.sort(function (a, b) {
        return a.index - b.index;
    });
    // --------------------------------------
}

function photosLoads(index) {
    var photo = photos[index].name;
    var basename = fileBreak(photo).base;

    // establece nombre y ruta, dependiendo si es proxy o no 
    if (project.proxy)
        var name = basename + "_proxy.jpg";
    else
        var name = basename + ".jpg";
    var file = path + "/footage/" + name;
    // ---------------------------------

    //elimina la foto si ya esta importada
    if (getItem(name))
        getItem(name).remove();
    // ---------------------------------

    // Carga las photos al proyecto after effect
    var imgComp = getComp("Image " + index);
    var io = new ImportOptions(File(file));
    var importedPhoto = app.project.importFile(io);
    imgComp.layers.add(importedPhoto)
    imgComp.layer(1).startTime = 0; // la deja en el primer frame
    // ---------------------------------
}

function disableSlides() {
    // desabilita las slide que no estan en el rango de las fotos que hay en el proyecto
    for (var i = 0; i < 50; i++) {
        var slide = slidesComp.layer("slide_" + i);
        if (slide != null)
            if (i < photos.length)
                slide.enabled = true;
            else
                slide.enabled = false;
    }
    // ---------------------------------------------------
}

function renderQueue() {
    // Cola de render
    var queue = app.project.renderQueue;
    // Borra todos los items de la cola
    while (queue.numItems > 0) {
        queue.item(queue.numItems).remove();
    }
    // -----------------------------------------------

    // Agrega la comp final a la cola
    var render = queue.items.add(finalComp);
    // ------------------------------------------

    // cambia las plantilla
    render.applyTemplate("videovina");
    render.outputModules[1].applyTemplate("h264");
    // ----------------------------------------
}

function timeModifier() {
    //Busca el tiempo final de la slide y pone una transicion a negro en la comp final
    var lastSlideName = "slide_" + (photos.length - 1);
    var lastSlide = slidesComp.layer(lastSlideName);
    endTime = lastSlide.startTime + lastSlide.source.duration;

    var opacity = slidesLayer.property("Opacity");

    opacity.setValueAtTime(endTime - transition, 100);
    opacity.setValueAtTime(endTime, 0);
    // ---------------------------------------------------

    // Cambio de tiempo
    var seg = 3;
    var stretch = (seg * 100) / 3;
    slidesLayer.stretch = stretch;
    // -------------------------------    

    //Duracion de composicion final
    finalComp.duration = endTime + 1;
    endFrame = parseInt((endTime + 1) * 30);
    // ---------------------------------------
}

function slideModifier(index) {
    var slide = getComp("slide_" + index);

    if (slide != undefined) {
        for (var e = 1; e <= slide.layers.length; e++) {
            var layer = slide.layer(e);
            // Cambia el color de todos los textos de la capa
            if (layer.name == "text")
                layer.effect("Fill")("Color").setValue(color);
            // -----------------------
        }
        // ------------------------------
    }
}

main();
