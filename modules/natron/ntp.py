from sys import argv
import NatronEngine
import os
import json
from util import fread, fwrite
from time import sleep

from slides import get_slides
from production import generate_production_slides
from vina import get_videovina

# datos de vinarender
data = json.loads(argv[2].replace("'", '"'))
base_project = data['project']
frame_range = data['frames']
slides_range = data['slides']
output_folder = data['output_folder']
# ----------------------

app = app1

base_project_name = os.path.basename(base_project)[:-4]

if not os.path.isdir(output_folder):
    os.makedirs(output_folder)
    os.system('chmod 777 -R ' + output_folder)

project_name = base_project_name + '_' + \
    str(frame_range[0]) + '-' + str(frame_range[1])

project = output_folder + '/' + project_name + '.ntp'

app.loadProject(base_project)

# al ejecutar simultaneamente este script, cuando se usa instancias,
# da conflicto al usar la funcion 'generate_production_slides' por eso
# se hace un mutex de archivo, cuando se esta usando el archivo tiene un '1'
# si no se esta ejectutando es '0'
mutex = '/tmp/mutex'
while(True):
    if not fread(mutex) == '1':
        fwrite(mutex, '1')
        generate_production_slides(None, app, app, slides_range, force=True)
        fwrite(mutex, '0')
        break
    sleep(0.1)

app.saveProjectAs(project)
