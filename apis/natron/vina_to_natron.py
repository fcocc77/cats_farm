import sys
import os
import json
from argparse import Namespace

from util import *

path = "/home/pancho/Documents/GitHub/vinarender/apis/natron"
# sys.path.insert(0, path)
# import vina_to_natron
# reload(vina_to_natron)
# vina_to_natron.app = app1
# vina_to_natron.main()

project_dir = "/mnt/server_01/videovina/as3/private/pancho/projects/test"
project_json = project_dir + "/project.json"
proj = jread(project_json)


project = Namespace(
    song=proj.states.app.song,
    color=proj.states.app.color,
    photos=proj.states.app.timeline,
    texts=proj.states.edit_items,
)


def main():
    color = app.Control.color

    update()
    images_connect()

    color.setValue(project.color[0] / 255.0, 0)
    color.setValue(project.color[1] / 255.0, 1)
    color.setValue(project.color[2] / 255.0, 2)


def images_connect():
    for photo in project.photos:
        _photo = project_dir + '/footage/' + photo.name
        reader = app.createReader(_photo)
        get_slide(photo.index).connectInput(0, reader)


def get_all_slide():
    slides = []

    for i in range(100):
        name = 'slide_' + str(i)
        slide = getattr(app, name, None)
        if slide:
            slides.append(slide)

    return slides


def get_slide(index):
    name = 'slide_' + str(index)
    slide = getattr(app, name, None)

    return slide


def update_slide(slide, index):
    # acualiza el tiempo del los nodos hijos

    slide.vIndex.setValue(index)
    time = app.Control.vTime.getValue()

    first_frame = (time * index) + 1
    last_frame = (time * index) + time

    slide.TimeOffset.timeOffset.setValue(first_frame-1)

    slide.FrameRange.frameRange.setValue(first_frame, 0)
    slide.FrameRange.frameRange.setValue(last_frame, 1)


def update():
    for index, slide in enumerate(get_all_slide()):
        # actualiza todas las slide
        update_slide(slide, index)
