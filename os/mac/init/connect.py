import os
from time import sleep


user="server1"
password="jump77cats"
folder="server_01"
ip="192.168.10.45"


mount_dir="/Volumes/"+folder
os.makedirs(mount_dir)

while 1:
	if os.listdir(mount_dir):
		break
	else:
		os.system("mount_smbfs //"+user+":"+password+"@"+ip+"/"+folder+" /Volumes/"+folder)
	sleep(1)

