#!/usr/bin/python 
import paramiko
import os

path=os.path.dirname(os.path.abspath(__file__))

class ssh():
	def __init__(self,ip,username,password,cmd):

			client=paramiko.SSHClient()
			client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
			client.connect(ip, username=username, password=password)

			stdin, stdout, stderr = client.exec_command(cmd)

			stdout=stdout.read()
			client.close()

			self.read = stdout

machines=["192.168.10.43", "192.168.10.45","192.168.10.46","192.168.10.47","192.168.10.48","192.168.10.49"]

for machine in machines:

	manager_name="192.168.10.45"

	if machine==manager_name:
		manager="y"
		server="y"
	else:
		manager="n"
		server="y"

	try:

		uninstall='python "'+path+'/setup.py" uninstall '+manager+" "+server+" "+manager_name
		print ssh(machine,"root","vfx",uninstall).read

		install='python "'+path+'/setup.py" install '+manager+" "+server+" "+manager_name
		print ssh(machine,"root","vfx",install).read
	except:
		print machine+" error"