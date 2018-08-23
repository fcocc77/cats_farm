#!/usr/bin/env python
import shutil
import os
from sys import platform
from sys import argv
import tarfile
from zipfile import ZipFile
import subprocess
from time import sleep
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
action = True
if platform == "linux2": debug = True
else: debug = False 
#-----------------------

# Checkea si hay argumentos
try:
	action = int( argv[1] )
	manager_start = int( argv[2] )
	server_start = int( argv[3] )
	ip = argv[4]
except: None
#-------------------------

def sh( cmd ):
	proc = subprocess.Popen( cmd, stdout=subprocess.PIPE , stderr=subprocess.PIPE, shell=True )
	return proc.communicate()[0]

def copydir( src, dst ):
	if not os.path.isdir( dst ): 
		shutil.copytree( src, dst )

def copyfile( src, dst ):
	if not os.path.isfile( dst ): 
		try: shutil.copy( src, dst )
		except: None

def fread( path ):
	if os.path.isfile( path ):
		f = open( path, "r" )
		read = f.read()
		f.close()
	else: read = "";

	return read

def fwrite( path, info ):
	f = open( path, "w" )
	f.write(info)
	f.close()

def compile_ ( project ):
	if platform == "linux2":
		qmake = "/opt/Qt5.7.1/5.7/gcc_64/bin/qmake"
	else: 
		qmake = "C:/Qt/Qt5.7.1/5.7/mingw53_32/bin/qmake.exe"
		make = "C:/Qt/Qt5.7.1/Tools/mingw530_32/bin/mingw32-make.exe"

	basename = fread( project ).split( "TARGET" )[1].split("\n")[0].strip().strip("=").strip().strip("\"")

	print "Compiling " + basename + "..."

	if debug:
		f = open(project, "a")
		f.write("\n\nCONFIG += debug")
		f.close()

	if platform == "win32":
		ProgramData = "C:/ProgramData/cats_farm/" + basename
		if not os.path.isdir( ProgramData ): os.makedirs( ProgramData );

		exe = ProgramData + "/release/" + basename + ".exe"

		if os.path.isfile(exe): os.remove(exe)

		sh( "cd " + ProgramData + " && " + qmake + " " + project )
		sh( "set path=" + os.path.dirname( make ) + "; && cd " + ProgramData + " && " + make )

		shutil.move( exe, windowsInstall + "/bin/win/" + basename + ".exe" )

	else:
		if debug: name = basename + "_debug"
		else: name = basename

		temp = "/var/tmp/" + name
		if not os.path.isdir( temp ): os.makedirs( temp );

		exe = temp + "/" + basename

		if os.path.isfile(exe): os.remove(exe)

		sh( "find " + linuxInstall + " -type f -exec touch {} +" ) # Aniade directorio de catsfarm para poder compilarlo
		sh( "find /opt/Qt5.7.1/ -type f -exec touch {} +" ) # Aniade directorio de QT5
		sh( "cd \"" + temp +"\" && " + qmake + " " + project )
		sh( "cd \"" + temp + "\" && source /opt/rh/devtoolset-7/enable && make" )

		shutil.move( exe, linuxInstall + "/bin/linux/" + basename )

	print "The " + basename + " compilation finishes."
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
		sh( "yum -y install " + rpms )
		#----------------------

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

	if manager_start: compile_( linuxInstall + "/code/manager/manager.pro" )
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
	sh( "sed -i -e 's/\r//g' /etc/init.d/cserver")
	sh( "sed -i -e 's/\r//g' /etc/init.d/cmanager")  
	#--------------------------------------------------------------------------------
	if server_start: os.system( "service cserver start" )
	if manager_start: os.system( "service cmanager start " )

	nuke_module(1)

def linux_uninstall():

	sh( "service cserver stop ")
	cserver =  "/etc/init.d/cserver"
	if os.path.isfile(cserver): os.remove( cserver )

	sh( "service cmanager stop ")
	cmanager =  "/etc/init.d/cmanager"
	if os.path.isfile(cmanager): os.remove( cmanager )

	if os.path.isdir(linuxInstall): shutil.rmtree(linuxInstall)

	desktop = "/usr/share/applications/CatsFarm.desktop"
	if os.path.isfile(desktop): os.remove(desktop)

	nuke_module(0)

def windows_install():
	print "---------------------------------------------"
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

	fwrite(  windowsInstall + "/etc/manager_host", ip )

	compile_( windowsInstall + "/code/server/server.pro" )
	compile_( windowsInstall + "/code/monitor/monitor.pro" )
	compile_( windowsInstall + "/code/manager/manager.pro" )
	compile_( windowsInstall + "/code/submit/submit.pro" )

	copyfile( windowsInstall + "/os/win/link/CatsFarm Monitor.lnk" , "C:/ProgramData/Microsoft/Windows/Start Menu/Programs")

	nssm = windowsInstall + "/os/win/service/nssm.exe" # para crear servicios

	sh( nssm + " install \"CatsFarm Server\" \"" + windowsInstall + "/bin/win/CatsFarm Server.exe\"" )
	sh( nssm + " install \"CatsFarm Manager\" \"" + windowsInstall + "/bin/win/CatsFarm Manager.exe\"" )
	#-------------------------------------

	# ingrsar clave de usuario
	user = sh("echo %username%").strip()
	for x in range(5):
		user_file = "C:/ProgramData/cats_farm/user"

		if x == 0: password = fread( user_file )
		else: password = raw_input( user + ' password: ')

		sh( nssm + " set \"CatsFarm Server\" ObjectName  \".\\" + user + "\" \""+ password + "\"" )
		login = sh( "sc start \"CatsFarm Server\"")

		if not "logon failure" in login:
			sh( nssm + " set \"CatsFarm Manager\" ObjectName  \".\\" + user + "\" \""+ password + "\"" )
			fwrite( user_file, password )
			break
		if x > 0: print "Wrong password..."
	#-----------------------------------------
	if server_start: sh( "sc start \"CatsFarm Server\"")
	else: 
		sh( "sc stop \"CatsFarm Server\"")
		sh( "sc config \"CatsFarm Server\" start= disabled" )

	if manager_start: sh( "sc start \"CatsFarm Manager\"")
	else: sh( "sc config \"CatsFarm Manager\" start= disabled" )
	#-------------------------------------

	# core temp
	sh( nssm + " install \"CatsFarm CoreTemp\" " + windowsInstall + "/os/win/core_temp/core_temp.exe" )
	sh( nssm + " start \"CatsFarm CoreTemp\"")
	#-----------------------------------------------------

	# sshd service
	OpenSSH = "C:/Program Files/OpenSSH"
	if not os.path.isdir( OpenSSH ):
		shutil.move( windowsInstall + "/os/win/OpenSSH", OpenSSH )
	sh( nssm + " install \"CatsFarm SSHD\" \"" + OpenSSH + "/bin/sshd_service.exe\"" )
	sh( nssm + " set \"CatsFarm SSHD\" ObjectName  \".\\" + user + "\" \""+ password + "\"" )
	sh( nssm + " start \"CatsFarm SSHD\"")
	#------------------------------------------------------------------

	# desbloquear puertos firewall
	sh( "netsh advfirewall firewall delete rule name=\"CatsFarm Ports:7000,7001\"" ) # delete
	sh( "netsh advfirewall firewall delete rule name=\"sshd Ports:22\"" ) # delete

	sh( "netsh advfirewall firewall add rule name=\"CatsFarm Ports:7000,7001\" dir=in action=allow enable=yes profile=Any protocol=TCP localport=7000,7001" )
	sh( "netsh advfirewall firewall add rule name=\"CatsFarm Ports:7000,7001\" dir=out action=allow enable=yes profile=Any protocol=TCP localport=7000,7001" )

	sh( "netsh advfirewall firewall add rule name=\"sshd Ports:22\" dir=in action=allow enable=yes profile=Any protocol=TCP localport=22" )
	sh( "netsh advfirewall firewall add rule name=\"sshd Ports:22\" dir=out action=allow enable=yes profile=Any protocol=TCP localport=22" )
	#------------------------------------------------------------------------------------

	nuke_module(1)

def windows_uninstall():

	nuke_module(0)

	# remove services
	nssm = windowsInstall + "/os/win/service/nssm.exe" # para crear servicios
	sh( nssm + " stop \"CatsFarm Server\"")
	sh( nssm + " remove \"CatsFarm Server\" confirm")
	sh( nssm + " stop \"CatsFarm Manager\"")
	sh( nssm + " remove \"CatsFarm Manager\" confirm")

	sh( nssm + " stop \"Core Temp\"") # borrar en un tiempo mas
	sh( nssm + " remove \"Core Temp\" confirm") # borrar en un tiempo mas

	sh( nssm + " stop \"CatsFarm CoreTemp\"")
	sh( nssm + " remove \"CatsFarm CoreTemp\" confirm")

	sh( nssm + " stop \"CatsFarm SSHD\"")
	sh( nssm + " remove \"CatsFarm SSHD\" confirm")

	sh( "taskkill -f -im \"CatsFarm Monitor.exe\"" );

	sleep( 0.1 )
	if os.path.isdir( windowsInstall ):
		sh("RD /S /Q \"" + windowsInstall + "\"" )
	lnk = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/CatsFarm Monitor.lnk"
	if os.path.isfile( lnk ):
		os.remove( lnk )

	if os.path.isdir( windowsInstall ): 
		return False
	else: return True

compiler_install()

if platform == "win32":
	if windows_uninstall(): windows_install()
	else:
		print "----------------------------"
		print "Some files are still in use."
		print "----------------------------"

elif platform == "linux2":
	if action:
		linux_uninstall()
		linux_install()
	else:
		linux_uninstall()

