app = app1


def get_all_slide():
    slides = []

    for i in range(100):
        name = 'slide_' + str(i)
        slide = getattr(app, name, None)
        if slide:
            slides.append(slide)

    return slides


def update_slide(slide):
    # acualiza el tiempo del los nodos hijos

    index = slide.vIndex.get()
    time = app.Control.vTime.getValue()

    first_frame = (time * index) + 1
    last_frame = (time * index) + time

    slide.TimeOffset.timeOffset.setValue(first_frame-1)

    slide.FrameRange.frameRange.setValue(first_frame, 0)
    slide.FrameRange.frameRange.setValue(last_frame, 1)


for slide in get_all_slide():
    # actualiza todas las slide
    update_slide(slide)
