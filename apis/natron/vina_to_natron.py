import os
import json
from argparse import Namespace

from util import *
import NatronEngine


path = "/home/pancho/Documents/GitHub/vinarender/apis/natron/project"

vina_project = "/mnt/server_01/videovina/as3/private/pancho/projects/test/project.json"
proj = jread(vina_project)


project = Namespace(
    song=proj.states.app.song,
    color=proj.states.app.color,
    images=proj.states.app.timeline,
    texts=proj.states.edit_items,
)


# carga proyecto natron
app.loadProject(path + "/test.ntp")


# conecta el nodo 'Render' al write creado
render_node = app.Render
writer = app.createWriter(path + "/render/test.mov")
writer.connectInput(0, render_node)


# Color
color = app.Color.color
color.setValue(project.color[0] / 255.0, 0)
color.setValue(project.color[1] / 255.0, 1)
color.setValue(project.color[2] / 255.0, 2)
