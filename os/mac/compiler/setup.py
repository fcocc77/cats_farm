import sys
import os
import shutil
from setuptools import setup

ICON_PNG="/usr/local/CatsFarm/icons/app.png"
ICON="/usr/local/CatsFarm/icons/app.icns"

# crea .icns a partir de un png
icns_make="sips -s format icns "+ICON_PNG+" --out "+ICON
os.system(icns_make)
#-------------------------------------------------------


BUILD_DIR="/tmp/build"
if not os.path.isdir(BUILD_DIR):
	os.mkdir(BUILD_DIR)

APP = ['/usr/local/CatsFarm/os/mac/compiler/CatsFarm Monitor.py']
DATA_FILES = []
OPTIONS = {'argv_emulation': True, 
			'iconfile':ICON,
			'dist_dir':'/Applications',
			'bdist_base':BUILD_DIR
			}

setup(
	app=APP,
	data_files=DATA_FILES,
	options={'py2app': OPTIONS},
	setup_requires=['py2app'],
)

os.rename("/Applications/CatsFarm Monitor.app","/Applications/CatsFarm.app")
os.system("chmod -R 777 /Applications/CatsFarm.app")
shutil.rmtree(BUILD_DIR)