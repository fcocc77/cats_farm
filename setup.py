#!/usr/bin/env python
import shutil
import os
from sys import platform
from sys import argv
import tarfile
from zipfile import ZipFile
import subprocess
path = os.path.dirname(__file__)

# Rutas de Instalacion
linuxInstall="/opt/cats_farm"
windowsInstall="C:/cats_farm"
macInstall="/usr/local/cats_farm"
#--------------------------

# Datos Generales
ip = "192.168.10.45"
manager_start = False
server_start = True
debug = False
#-----------------------

# Checkea si hay argumentos
try:
	action = int( argv[1] )
	manager_start = int( argv[2] )
	server_start = int( argv[3] )
	ip = argv[4]
except: None
#-------------------------

def copydir( src, dst ):
	if not os.path.isdir( dst ): 
		shutil.copytree( src, dst )

def copyfile( src, dst ):
	if not os.path.isfile( dst ): 
		shutil.copy( src, dst )

def compile_ ( project ):
	if platform == "linux2":
		qmake = "/opt/Qt5.7.1/5.7/gcc_64/bin/qmake"
	else: 
		qmake = "C:/Qt/Qt5.7.1/5.7/mingw53_32/bin/qmake.exe"
		make = "C:/Qt/Qt5.7.1/Tools/mingw530_32/bin/mingw32-make.exe"

	dirname = os.path.dirname(project).replace("\\","/")
	basename = os.path.basename(dirname)

	print "Compiling " + dirname + "..."

	if debug:
		f = open(project, "a")
		f.write("\n\nCONFIG += debug")
		f.close()

	if platform == "win32":
		ProgramData = "C:/ProgramData/cats_farm/" + basename
		if not os.path.isdir( ProgramData ): os.makedirs( ProgramData );

		exe = ProgramData + "/release/" + basename + ".exe"

		if os.path.isfile(exe): os.remove(exe)

		os.system("cd " + ProgramData + " && " + qmake + " " + project + " > %temp%/null" )
		os.system( "set path=" + os.path.dirname( make ) + "; && cd " + ProgramData + " && " + make + " > %temp%/null")

		shutil.move( exe, windowsInstall + "/bin/win/" + basename + ".exe" )

	else:
		if debug: name = basename + "_debug"
		else: name = basename

		temp = "/var/tmp/" + name
		if not os.path.isdir( temp ): os.makedirs( temp );

		exe = temp + "/" + basename

		if os.path.isfile(exe): os.remove(exe)

		os.system("cd " + temp +" && " + qmake + " "+project + " &> /dev/null")
		os.system("cd " + temp + " && source /opt/rh/devtoolset-7/enable && make > /dev/null")

		shutil.move( exe, linuxInstall + "/bin/linux/" + basename )

	print "Finished Compler."
	print "---------------------------------------------"

def compiler_install():
	if platform == "win32":
		if not os.path.isdir( "C:/Qt" ):
			zf = ZipFile( path + "/qt/win/Qt5.7.1.zip", "r" )
			zf.extractall( "C:/" )

	if platform == "linux2":
		if not os.path.isdir( "/opt/Qt5.7.1" ):
			# untar QT5
			tar = tarfile.open( path + "/qt/linux/Qt5.7.1.tar.gz" )
			for i in tar:
				tar.extract(i, path = "/opt")
			tar.close()
			#----------------------

		# install rpms nesesarios
		rpms = ""
		for rpm in os.listdir( path + "/qt/linux/libs" ):
			rpms +=  path + "/qt/linux/libs/" + rpm + " "
		os.system( "yum -y install " + rpms + " &> /dev/null" )
		#----------------------

def sublime_build():
	if platform == "win32": 
		proc = subprocess.Popen( "%username%", stdout=subprocess.PIPE , stderr=subprocess.PIPE, shell=True)
		user = proc.communicate()[1].split("'")[1]

		dirs = "C:/Users/" + user + "/AppData/Roaming/Sublime Text 3/Packages/User"

		f = open( dirs + "/QT5-C++.sublime-build", "w" )
		f.write( '{ "cmd":["python", "' + dirs + '/QT5-C++.py","${file}"]}' )
		f.close()

		copyfile( path + "/modules/QT5-C++.py", dirs )

	else: 
		user = os.listdir("/home")[0]
		dirs = "/home/" + user + "/.config/sublime-text-3/Packages/User"

		f = open( dirs + "/QT5-C++.sublime-build", "w" )
		f.write( '{ "cmd":["python", "' + dirs + '/QT5-C++.py","${file}"]}' )
		f.close()

		copyfile( path + "/modules/QT5-C++.py", dirs )

		os.system( "chmod 777 " + dirs + "/QT5-C++.sublime-build" )
		os.system( "chmod 777 " + dirs + "/QT5-C++.py" )

def nuke_module(install):
	pluginsSys=["/usr/local/nuke/plugins","C:/Program Files/nuke/plugins","/Applications/nuke/nuke.app/Contents/MacOS/plugins"]

	for a in range(9,11):
		for b in range(10):
			for s in pluginsSys:
				for d in ["0","5"]:
					nukeVersion="Nuke"+str(a)+"."+d+"v"+str(b)
					plugins=s.replace("nuke",nukeVersion)

					if os.path.isdir(plugins):

						menuPy=plugins+"/menu.py"

						if os.path.isfile(menuPy):

							l1="import nukeCatsFarm\n"
							l2='menuBarCat = nuke.menu("Nuke")\n'
							l3="menuBarCat.addCommand('J_Script/SendToCatsfarm...', 'nukeCatsFarm.catsFarmSend()', 'Ctrl+r')\n"

							if install:
								f=open(menuPy,"a")
								f.write(l1)
								f.write(l2)
								f.write(l3)
								f.close()

								shutil.copy(path+"/modules/nukeCatsFarm.py",plugins)

							else:
								nukeCatsFarm=plugins+"/nukeCatsFarm.py"
								if os.path.isfile(nukeCatsFarm):
									os.remove(nukeCatsFarm)

								f=open(menuPy,"r")
								lines=f.read()
								f.close()

								s=lines.replace(l1,"").replace(l2,"").replace(l3,"")

								f=open(menuPy,"w")
								f.write(s)
								f.close()

						#--------------------------------------------------------------

def linux_install():
	if not os.path.isdir(linuxInstall):
		os.mkdir( linuxInstall )

	# copia el contenido necesario
	copydir( path + "/bin/linux", linuxInstall + "/bin/linux" )
	copydir( path + "/code", linuxInstall + "/code" )
	copydir( path + "/etc", linuxInstall + "/etc" )
	copydir( path + "/icons", linuxInstall + "/icons" )
	copydir( path + "/log", linuxInstall + "/log" )
	copydir( path + "/os/linux", linuxInstall + "/os/linux" )
	copydir( path + "/sound", linuxInstall + "/sound" )
	copydir( path + "/modules", linuxInstall + "/modules" )
	copydir( path + "/theme", linuxInstall + "/theme" )
	#-----------------------------------------------------

	compile_( linuxInstall + "/code/manager/manager.pro" )
	compile_( linuxInstall + "/code/monitor/monitor.pro" )
	compile_( linuxInstall + "/code/server/server.pro" )
	compile_( linuxInstall + "/modules/denoiser/denoiser.pro" )

	copyfile( path + "/os/linux/link/CatsFarm.desktop" , "/usr/share/applications")

	shutil.copy( linuxInstall + "/os/linux/init/cserver", "/etc/init.d/cserver")
	shutil.copy( linuxInstall + "/os/linux/init/cmanager", "/etc/init.d/cmanager")

	f = open( linuxInstall + "/etc/manager_host" , "w" )
	f.write( ip )
	f.close()

	if debug:dbug="true"
	else:dbug="false"
	f = open( linuxInstall + "/etc/debug", "w" )
	f.write(dbug)
	f.close()
	# los servicios son muy estrictos asi esto corrige el servicio si de modifico mal
	os.system( "sed -i -e 's/\r//g' /etc/init.d/cserver")
	os.system( "sed -i -e 's/\r//g' /etc/init.d/cmanager")  
	#--------------------------------------------------------------------------------
	os.system( "service cserver start &>/dev/null" )
	os.system( "service cmanager start &>/dev/null" )

	nuke_module(1)

def linux_uninstall():

	os.system( "service cserver stop ")
	cserver =  "/etc/init.d/cserver"
	if os.path.isfile(cserver): os.remove( cserver )

	if os.path.isdir(linuxInstall): shutil.rmtree(linuxInstall)

	desktop = "/usr/share/applications/CatsFarm.desktop"
	if os.path.isfile(desktop): os.remove(desktop)

	nuke_module(0)

def windows_install():
	if not os.path.isdir(windowsInstall):
		os.mkdir( windowsInstall )

	# copia el contenido necesario
	copydir( path + "/bin/win", windowsInstall + "/bin/win" )
	copydir( path + "/code", windowsInstall + "/code" )
	copydir( path + "/etc", windowsInstall + "/etc" )
	copydir( path + "/icons", windowsInstall + "/icons" )
	copydir( path + "/log", windowsInstall + "/log" )
	copydir( path + "/os/win", windowsInstall + "/os/win" )
	copydir( path + "/sound", windowsInstall + "/sound" )
	copydir( path + "/modules", windowsInstall + "/modules" )
	copydir( path + "/theme", windowsInstall + "/theme" )
	#-----------------------------------------------------

	f = open( windowsInstall + "/etc/manager_host" , "w" )
	f.write( ip )
	f.close()

	compile_( windowsInstall + "/code/server/server.pro" )
	compile_( windowsInstall + "/code/monitor/monitor.pro" )
	compile_( windowsInstall + "/code/manager/manager.pro" )
	compile_( windowsInstall + "/code/submit/submit.pro" )
	compile_( windowsInstall + "/modules/denoiser/denoiser.pro" )
	
	copyfile( windowsInstall + "/os/win/link/CatsFarm Monitor.lnk" , "C:/ProgramData/Microsoft/Windows/Start Menu/Programs")

	nuke_module(1)

def windows_uninstall():
	if os.path.isdir( windowsInstall ):
		os.system("RD /S /Q \"" + windowsInstall + "\"" )
	lnk = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/CatsFarm Monitor.lnk"
	if os.path.isfile( lnk ):
		os.remove( lnk )

	nuke_module(0)

compiler_install()
#sublime_build()

if platform == "win32":
	windows_uninstall()
	windows_install()
elif platform == "linux2":
	if action:
		linux_uninstall()
		linux_install()
	else:
		linux_uninstall()

