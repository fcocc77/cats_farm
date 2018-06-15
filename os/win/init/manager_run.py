#!/usr/bin/python

import os
import sys
import subprocess
import signal
import psutil

path=os.path.dirname(os.path.abspath(__file__))
installFolder= os.path.dirname(os.path.dirname(os.path.dirname(path)))

sys.path.insert(0,installFolder+'/code')
from util import kill

try: 
	action = sys.argv[1]

except:
	action=""
	print ("Write: start or stop")

def start():
	cmd=installFolder+"/code/manager.exe > "+installFolder+"/log/manager_log"
	started = subprocess.Popen(cmd,shell=True)

	f=open(path+"/managerPID","w")
	f.write(str(started.pid))
	f.close

	print ("CatsFarm Manager: has started")

def stop():
	file=path+"/managerPID"
	if os.path.isfile(file):
		f=open(file,"r")
		pid = int(f.readline())
		f.close

		kill(pid)

	print ("CatsFarm Manager: has stoped")

if action =="start":
	stop()
	start()

if action =="stop":
	stop()