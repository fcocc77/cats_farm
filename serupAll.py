#!/usr/bin/python 
import paramiko
import os

path=os.path.dirname(os.path.abspath(__file__))
path = path.replace("\\","/").replace("/192.168.10.45","mnt")

class ssh():
	def __init__(self,ip,username,password,cmd):

			client=paramiko.SSHClient()
			client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
			client.connect(ip, username=username, password=password)

			stdin, stdout, stderr = client.exec_command(cmd)

			stdout=stdout.read()
			client.close()

			self.read = stdout

servers=["192.168.10.45","192.168.10.46","192.168.10.47","192.168.10.48","192.168.10.49", "192.168.10.52"]

for _server in servers:

	manager_name="192.168.10.45"

	if _server==manager_name:
		manager="1"
		server="1"
	else:
		manager="0"
		server="1"

	try:
		print _server + ": --------------------"
		print "	- Uninstalling..."
		uninstall = 'python "'+path+'/setup.py" 0 ' + manager+" "+server+" "+manager_name
		ssh(_server,"root","vfx",uninstall).read
		print "	- Installing..."
		install='python "'+path+'/setup.py" 1 '+manager+" "+server+" "+manager_name
		ssh(_server,"root","vfx",install)
		print "-----------------------------------" 
	except:
		print _server+" error"