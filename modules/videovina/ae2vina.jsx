//@include "utils.jsx"

function textsEnabled(slide, enable) {
    for (var e = 1; e <= slide.layers.length; e++) {
        var layer = slide.layer(e);
        if (layer.name == "text")
            layer.enabled = enable;
    }
}

function stencilLayers(slide, stencil) {
    for (var e = 1; e <= slide.layers.length; e++) {
        var layer = slide.layer(e);

        // cambia el blending mode a la foto y del texto
        if (layer.name == "image" || layer.name == "text") {
            if (stencil)
                layer.blendingMode = 4042; // 4042 es Stencil Alpha
            else
                layer.blendingMode = 4012; // 4012 es Normal
        }
    }
}

function exportSlides() {
    var slidesComp = getComp("Slides");

    for (var i = 0; i < 2; i++) {
        var slideName = "slide_" + i;
        var slide = getComp(slideName);
        var layerSlide = slidesComp.layer(slideName);
        var render = app.project.renderQueue.items.add(slidesComp);

        // pone las capas de photo en stencil
        stencilLayers(slide, true);
        textsEnabled(slide, false);
        // --------------------------------------------

        // cambia la plantilla
        render.outputModules[1].applyTemplate("png_alpha");
        // ----------------------------------------

        //guarda un frame justo el de la mitad de cada composicion
        render.timeSpanStart = layerSlide.startTime + (slide.duration / 2);
        render.timeSpanDuration = 1 / 30;

        // Cambia el nombre de la salida
        render.outputModule(1).file = new File(path + "/samples/slide_" + i + ".png");
        // ------------------------------------

        app.project.renderQueue.render();
        //una vez rendereada borra el item de render
        render.remove()
        // ----------------------------------------
        stencilLayers(slide, false);
        textsEnabled(slide, true);
    }

}

function getTextTransform() {
    var items = [];
    for (var i = 0; i < 10; i++) {
        var slide = getComp("slide_" + i);
        var text = slide.layer("text");

        // quita el parent al texto para guardar los datos de transformar puros,
        //luego los guarda en una variable para volver a emparentarlo al final
        var parent = text.parent;
        text.parent = null;
        // ------------------------------------

        var x = parseInt(text.position.value[0]);
        var y = parseInt(text.position.value[1]);

        var position = [x, y];
        var scale = parseInt(text.scale.value[0]);
        var rotation = text.rotation.value;

        items.push({
            "name": "slide_0.png",
            "checked": true,
            "expanded": true,
            "h1": "Titulo",
            "h2": "Sub Titulo",
            "img": "",
            "index": i,
            "text": {
                "position": position,
                "scale": scale,
                "rotation": rotation
            }
        });

        // enparentar
        text.parent = parent;
        // --------------------------------

        project.edit.items = items;
        jwrite(projectFile, project);
    }
}

exportSlides();