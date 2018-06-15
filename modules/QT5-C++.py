import os
import sys
import shutil
import time
from sys import platform
import subprocess

if platform == "linux2":
	qmake = "/opt/Qt5.7.1/5.7/gcc_64/bin/qmake"
else: 
	qmake = "C:/Qt/Qt5.7.1/5.7/mingw53_32/bin/qmake.exe"
	make = "C:/Qt/Qt5.7.1/Tools/mingw530_32/bin/mingw32-make.exe"
	gdb = "C:/Qt/Qt5.7.1/Tools/mingw530_32/bin/gdb.exe"

cpp = sys.argv[1]
dirname = os.path.dirname(cpp).replace("\\","/")
basename = os.path.basename(dirname)

# encuentra el projecto en la carpeta del cpp
project = None
for file in os.listdir(dirname):
	ext = file.split(".")[-1]
	if ext == "pro":
		project = dirname + "/" + file
#--------------------------------------------

# checkea si el projecto tiene debug
debug = False
f = open( project, "r" )
for l in f.readlines():
	if "debug" in l:
		if not "#" in l:
			debug = True
#------------------------------------


remove_files = [ "Makefile", "Makefile.Debug", "Makefile.Release", ".qmake.stash", "object_script.monitor.Debug", "object_script.monitor.Release" ]

if platform == "win32":
	exe = dirname + "/" + basename + ".exe"

	if os.path.isfile(exe): os.remove(exe)

	os.system("cd " + dirname + " && " + qmake + " " + project + " > %temp%/null" )
	os.system( "set path=" + os.path.dirname( make ) + "; && cd " + dirname + " && " + make + " > %temp%/null")

	# elimina archivos basura
	for f in remove_files:
		f = dirname + "/" + f
		if os.path.isfile(f): os.remove(f)
	#--------------------------------------

	# ejecuta el .exe
	if os.path.isfile(exe): 
		if debug:
			cmd_gdb = "echo y | \"" + gdb +"\" --eval-command=run --eval-command=quit  --args "
			cmd = cmd_gdb + exe
		else:
			cmd = exe

		os.system(cmd)

	#-----------------------

else:
	exe = dirname + "/" + basename

	if os.path.isfile(exe): os.remove(exe)

	os.system("cd " + dirname +" && " + qmake + " "+project + " &> /dev/null")
	os.system("cd " + dirname + " && source /opt/rh/devtoolset-7/enable && make > /dev/null")

	# elimina archivos basura
	for f in remove_files:
		f = dirname + "/" + f
		if os.path.isfile(f): os.remove(f)
	#--------------------------------------

	os.system(exe)