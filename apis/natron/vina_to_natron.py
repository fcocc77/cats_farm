import sys
import os
import json
from argparse import Namespace

from util import *

path = "/home/pancho/Documents/GitHub/vinarender/apis/natron"


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

    texts()

    color.setValue(project.color[0] / 255.0, 0)
    color.setValue(project.color[1] / 255.0, 1)
    color.setValue(project.color[2] / 255.0, 2)


def fit_texts(slide):
    x = 1920
    y = 1080
    max_height = y / 2

    def font_resize(text):
        # re escala el tamaño de la fuente hasta que quede
        # del ancho del cuadro
        text.size.setValue(0)

        size = 0
        width = 0
        height = 0
        while(width < x and height < max_height):
            size += 1
            text.size.setValue(size)
            width = text.getRegionOfDefinition(1, 1).x2
            height = text.getRegionOfDefinition(1, 1).y2

        return [width, height]

    title_x, title_y = font_resize(slide.title)
    subtitle_x, subtitle_y = font_resize(slide.subtitle)

    # calcula el alto total, para poder centrar los 2 textos al cuadro
    height = title_y + subtitle_y
    move_up = (y - height) / 2

    # ajusta los textos verticalmente
    slide.title_position.translate.setValue(subtitle_y + move_up, 1)
    slide.subtitle_position.translate.setValue(move_up, 1)

    # centra los textos horizontalmente
    slide.title_position.translate.setValue(
        (x / 2) - (title_x / 2), 0
    )
    slide.subtitle_position.translate.setValue(
        (x / 2) - (subtitle_x / 2), 0
    )


def texts():
    for index, slide in enumerate(get_all_slide()):
        index = str(index + 1)
        text = getattr(project.texts, 'text' + index, None)

        if text:
            slide.title.text.setValue(text.title)
            slide.subtitle.text.setValue(text.subtitle)
        else:
            slide.title.text.setValue('')
            slide.subtitle.text.setValue('')

        # cambia el nombre, ya que natron da problemas de cache, al
        # tener el mismo nombre en los nodos title y subtitle
        slide.title.setScriptName('title' + index)
        slide.subtitle.setScriptName('subtitle' + index)


def images_connect():
    for photo in project.photos:
        _photo = project_dir + '/footage/' + photo.name
        reader = app.createReader(_photo)
        get_slide(photo.index + 1).connectInput(0, reader)


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


def slide_range(slide, index):
    # acualiza el tiempo del los nodos hijos

    time = app.Control.vTime.getValue()

    first_frame = (time * index) + 1
    last_frame = (time * index) + time

    slide.TimeOffset.timeOffset.setValue(first_frame-1)

    slide.FrameRange.frameRange.setValue(first_frame, 0)
    slide.FrameRange.frameRange.setValue(last_frame, 1)


def update():
    for index, slide in enumerate(get_all_slide()):
        # actualiza todas las slide
        slide_range(slide, index)
