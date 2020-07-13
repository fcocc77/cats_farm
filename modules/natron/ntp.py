from sys import argv
import NatronEngine
import os

from slides import get_slides
from production import generate_production_slides
from natron_extent import get_videovina

app = app1

base_project = '/home/pancho/Desktop/test/comp/test.ntp'


app.loadProject(base_project)


videovina = get_videovina()
generate_production_slides(videovina, app, app, [2, 4], force=True)


version_project = '/home/pancho/Desktop/test/comp/test_pro.ntp'
app.saveProjectAs(version_project)

# for slide in get_slides(app):
#     print slide['slide'].getLabel()
