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
ip = "192.168.10.43"
manager_start = False
server_start = True
debug = True
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

def linux_uninstall():

	os.system( "service cserver stop ")
	cserver =  "/etc/init.d/cserver"
	if os.path.isfile(cserver): os.remove( cserver )

	if os.path.isdir(linuxInstall): shutil.rmtree(linuxInstall)

	desktop = "/usr/share/applications/CatsFarm.desktop"
	if os.path.isfile(desktop): os.remove(desktop)

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
	compile_( windowsInstall + "/modules/denoiser/denoiser.pro" )

	copyfile( windowsInstall + "/os/win/link/CatsFarm Monitor.lnk" , "C:/ProgramData/Microsoft/Windows/Start Menu/Programs")

def windows_uninstall():
	if os.path.isdir( windowsInstall ):
		os.system("RD /S /Q \"" + windowsInstall + "\"" )
	lnk = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/CatsFarm Monitor.lnk"
	if os.path.isfile( lnk ):
		os.remove( lnk )

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

