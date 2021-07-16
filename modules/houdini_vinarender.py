import sys
import os

project = sys.argv[1]
node_path = sys.argv[2]
first_frame = int(sys.argv[3])
last_frame = int(sys.argv[4])

hou.hipFile.load(project)

node = hou.node(node_path)

if not node:
    print("error: " + node_path + " - Node not found")
    sys.exit()


node_type = node.type().name()


def check_pre_render(filename_param):

    output_dir = os.path.dirname(node.parm(filename_param).eval())

    if not os.path.isdir(output_dir):
        print("error: " + output_dir + " - directory not found")
        sys.exit()


def check_post_render(filename_param):

    frames = last_frame - first_frame + 1
    finded_count = 0

    for i in range(first_frame, last_frame + 1):
        hou.setFrame(i)
        filename = node.parm(filename_param).eval()

        if os.path.isfile(filename):
            finded_count += 1

    if not finded_count == frames:
        print('error: ' + str(finded_count) + ' of ' +
              str(frames) + ' frames were rendered')

        sys.exit()


if node_type == 'filecache':
    check_pre_render('file')

    node.parm('f1').deleteAllKeyframes()
    node.parm('f2').deleteAllKeyframes()

    node.parm('f1').set(first_frame)
    node.parm('f2').set(last_frame)

    node.parm('savebackground').set(False)

    node.parm("execute").pressButton()

    check_post_render('file')

elif node_type == 'opengl':
    check_pre_render('picture')

    node.parm('trange').set(1)

    node.parm('f1').set(first_frame)
    node.parm('f2').set(last_frame)

    node.parm("execute").pressButton()

    check_post_render('picture')
