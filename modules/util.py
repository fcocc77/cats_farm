import os
from time import sleep
import socket
import sys
from sys import platform, version_info
import shutil
import ctypes
import signal
import pickle
import json
import random
import string
import threading
import subprocess
try:import psutil
except:None
# cuando se compila a executable la ruta cambia, por esto path cambia dependiendo si es  .py o .exe
if getattr(sys, 'frozen', False):
    if platform=="darwin":
    	path=os.path.dirname(os.path.abspath(__file__))
    else:
    	path=os.path.dirname(sys.executable)
elif __file__:
	path=os.path.dirname(os.path.abspath(__file__))	
path=os.path.dirname(path)

def fwrite(file,date):
	if not date:
		date="None"
	if date=="void":
		date=""

	try:
		f=open(file,"w")
		f.write(date)
		f.close()
	except:None

def fread(file):

	reading="0"
	for n in range(100):

		if os.path.isfile(file):
			f=open(file,"r")
			reading=str(f.read().strip())
			f.close()

			if reading:break

		else:break

		sleep(0.07)

	return reading

def jread(file):
	if os.path.isfile(file):
		with open(file, "r") as jsonFile:
			return json.load(jsonFile)
	else:return None

def jwrite(file,data):
	for f in range(10):
		try:info=json.dumps(data, sort_keys=True, indent=4)
		except:info=None

		if info:
			f=open(file,"w")
			f.write(info)
			f.close()
			break

class concat():
	def __init__(self,folder):
		if platform=="win32":
			ffmpeg=path+"/os/windows/ffmpeg/ffmpeg.exe"
			logMetod=" > "
		if platform=="linux2":
			ffmpeg="ffmpeg"
			logMetod=" &> "

		dirMovie=os.path.dirname(folder)

		list=dirMovie+'/list_'+os.path.basename(folder)

		try:
			files=sorted(os.listdir(folder))
			name=files[0]
			movie=dirMovie+"/"+(name.replace(name.split("_")[-1],"")[:-1]+".mov")
		except:
			files=[]
			movie="None"

		### crea lista de archivos para el concat		
		f = open(list,'w')
		for file in files:
			line = "file '"+folder+"/"+file+"'"			
			f.write(line+'\n') 
		f.close()
		#----------------------------------

		if platform=="win32":
			safe="-safe 0"
		else:
			safe=""

		### une todos los videos separados en uno		
		concat =ffmpeg+' -y -f concat '+safe+' -i "'+list+'" -c copy "'+movie+'"'

		if platform=="win32":
			concat=concat.replace("/","\\")

		os.system(concat+logMetod+'"'+dirMovie+'/null"')

		try:os.remove(dirMovie+"/null")
		except:None
		#-----------------------------------------------------

		### borra lista creada
		try:os.remove(list)
		except:None
		#----------------------

		### borra carpeta de los videos separados
		try:shutil.rmtree(folder) 
		except:None
		#----------------------------------------

class rootUser():
	def __init__(self):

		# Ceckerar si hay privilegios de administrado
		try:
			root = os.getuid() == 0
		except AttributeError:
			root = ctypes.windll.shell32.IsUserAnAdmin() != 0
		#-------------------------------------------------------

		self.read=root

class getInfo():
	def __init__(self):
		manager_ip=fread(path+"/etc/manager_host")

		#tcpClient(host,port,loop,update,thread,funtion or package)
		info = tcpClient(host=manager_ip, port=7772, loop=0, sleep=1, background=0, pks="servers").read
		#----------------------------------

		server_info=info[0]
		group_info=info[1]

		# Obtiene los grupos del Manager
		self.serverGroup=[]
		for group_name in group_info:
			if not group_name =="":
				self.serverGroup.append(group_name)
		#------------------------------------------

		# Obtiene los server del Manager
		self.server=[]
		for server_name in server_info:
			if not server_name =="":
				self.server.append(server_name)
		#------------------------------------------

def sh(command):
	proc = subprocess.Popen(command, stdout=subprocess.PIPE , stderr=subprocess.PIPE, shell=True)
	return proc.communicate()[0]

def kill(pid):

	if platform =="linux2":
		pids=[]
		childs=[pid]
		pids.append(pid)

		while 1:

			for child in childs:

				result=os.popen("ps --ppid "+str(child)).read()

				childs=[]
				for line in range(1,77):
					try:
						pid=result.split("\n")[line].split()[0]
						childs.append(pid)
					except:break

			if childs:
				for c in childs:

					pids.append(int(c))

			else:	
				break

		for p in pids:
			sh("kill "+str(p))

		return pids

	else:
		try:
			parent = psutil.Process(pid)
			for child in parent.children(recursive=True):  # or parent.children() for recursive=False
				child.kill()
			parent.kill()
		except:pass

def getIp():

	if platform=="win32":

		try:host=sh("ipconfig").split("IPv4 Address")[-1].split("\n")[0].split(" ")[-1].strip()
		except:host=""

		if host[:3]=="192":
			return host
		else:
			try:return socket.gethostbyname(socket.gethostname())
			except:return None

	if platform=="linux2":
		try:host=socket.gethostbyname(socket.gethostname())
		except:host=None 		
		if host:
			return host
		try:return sh("ifconfig").split("eth0")[-1].split("inet addr:")[1].split(" ")[0]
		except:return None

	else:
		try:return sh("ifconfig").split("en0:")[-1].split("inet ")[1].split(" ")[0]
		except: None
		try:return socket.gethostbyname(socket.gethostname())
		except:return None

class tcpClient():
	def __init__(self,host=None, port=None, loop=None, sleep=None, background=None, timeout=None, pks=None):

		if background:
			threading.Thread(target=self.hilo, args = (host, port, loop, sleep, timeout, pks)).start()
		else:
			self.hilo(host,port,loop,sleep,timeout,pks)

	def hilo(self,host,port,loop,sleepy,timeout,pks):

		# si es una funcion la en via si no recive el paquete
		if  str(pks)[0] =="<":
			funtion=1
		else:
			funtion=0
		#----------------------------------------------------
		self.status=False
		firstLoop=0
		recv=False
		while 1:
			s = socket.socket()
			if timeout:
				s.settimeout(timeout)
				try:
					self.status=True
					s.connect((host, port))
				except:
					self.status=False

			else:
				try:s.connect((host, port))
				except:None

			while 1:
				if  funtion:
					pks_send=pks(recv)
				else:
					pks_send=pks

				# desclosa el paquete para poder enviarlo
				pks_send=pickle.dumps(pks_send)
				# ------------------------------------

				size_send=str(len(pks_send))

				try:
					s.send(size_send.encode()) # 1
					s.recv(64) # 4

					s.send(pks_send) # 5
					size_recv = s.recv(64) # 8

					#--------------------------------------------------

					s.send(size_recv) # 9
					size_recv=size_recv.decode()
					recv="".encode()

					while 1:

						pks_recv = s.recv(1024) # 12
						recv=recv+pks_recv
						localSize=str(len(recv))

						if size_recv==localSize:break
						if not pks_recv:break

					#--------------------------------------------------

					# compone el paquete para poder enviarlo
					recv=pickle.loads(recv)
					# ------------------------------------

				except:
					break

				if loop and funtion:

					if firstLoop:
						sleep(sleepy)
					else:
						firstLoop=1

				else:
					break

			s.close()

			if loop and funtion:
				sleep(sleepy)
			else:break

		self.read=recv

class tcpServer():
	def __init__(self,host=None, port=None, background=None, pks=None):

		if background:
			threading.Thread(target=self.hilo, args = (host, port, pks)).start()
		else:
			self.hilo(host,port,pks)

	def hilo(self,host,port,object):
		while 1:
			threads=threading.Thread(target=self.hilo2, args = (host, port, object))
			threads.start()
			while 1:
				sleep(2)
				host_new=getIp()
				if not host_new==host:
					host=host_new
					threads._Thread__stop()
					break

			sleep(2)

	def hilo2(self,host,port,object):
		# cuando no esta conectada la red, el socket da error de coneccion,asi que queda en loop
		# hasta que se pueda conectar, una vez conectado ya no hay problemas de coneccion aun que se desconecte
		while 1:
			try:
				s = socket.socket()
				s.bind((host, port))
				s.listen(100)
				break
			except:
				sleep(2)
		#------------------------------------------------------

		while True:
			conn, addr = s.accept()
			threading.Thread(target=self.update, args = (conn, object)).start()

	def update(self,conn,object):

		send="None"
		while 1:

			try:
				size_recv = conn.recv(64) # 2
				conn.send(size_recv) # 3
				size_recv=size_recv.decode()
			except:break

			recv="".encode()
			while 1:
				try:
					pks_recv = conn.recv(1024) # 6
					recv=recv+pks_recv
					localSize=str(len(recv))

					if size_recv==localSize:break
					if not pks_recv:break
				except:break

			if recv: 
				# compone el paquete para poder enviarlo
				try:recv=pickle.loads(recv)
				except:None
				# ------------------------------------

				pks_send_2=object(recv)

				# desclosa el paquete para poder enviarlo, pickle da un error al cambiar el tamano de archivo
				pks_send=""
				for n in range(20):

					try:
						pks_send=pickle.dumps(pks_send_2)
						break
					except:

						if n==20:
							print ("Socket Pickle, error 20 time tried")
						sleep(0.07)
				# ------------------------------------

				size_send=str(len(pks_send))

				try:conn.send(size_send.encode()) # 7
				except:None

				try:conn.recv(64) # 10
				except:None

				try:conn.send(pks_send)
				except:None
				 # 11

			if not size_recv:
				break

		conn.close()

class resources():
	def __init__(self):
		self.temp=0
		self.temp_mac_running=False

		if platform=="linux2":
			mem=fread("/proc/meminfo")
			total=float(mem.split("MemTotal:")[1].split("kB")[0])		
			self.total_ram=int(round(total/1000000))
			self.count_cpu=int(sh("nproc"))
			self.proc_stat_isrunning=False

	def cpu_percent(self):
		return int(psutil.cpu_percent())

	def cpu_count(self):

		if platform=="linux2":
			return self.count_cpu
		else:
			return int(psutil.cpu_count())

	def cpu_temp(self):
		if platform=="linux2":
			try:
				temp_out = sh("sensors")
				core0 =  temp_out.split("Core 0:")[-1].split("+")[1].split(".")[0]
				core1 =  temp_out.split("Core 1:")[-1].split("+")[1].split(".")[0]
				core2 =  temp_out.split("Core 2:")[-1].split("+")[1].split(".")[0]
				core3 =  temp_out.split("Core 3:")[-1].split("+")[1].split(".")[0]
				self.temp = round((int(core0)+int(core1)+int(core2)+int(core3))/4)
			except:None

		elif platform=="win32":
			# Obtiene la temperature a partir de un sensor con interface core temp, no es lo mas optimo hasta el momento
			core_temp_dir=path+"/os/windows/core_temp"

			try:
				csv=[]
				for file in os.listdir(core_temp_dir):
					ext= file.split(".")[-1]	
					if ext =="csv":
						f=open(core_temp_dir+"/"+file,"r")
						temp_lines=f.readlines()
						f.close()
						csv.append(core_temp_dir+"/"+file)

				if len(temp_lines) > 50:
					for c in csv:
						os.remove(c)

				temp_line= temp_lines[-1].split(",,,")[0].split(",")[1:]
				cores=0
				for n in temp_line:
					cores+=int(n)

				self.temp=round(cores/len(temp_line))
			except:None
			#-------------------------------------------------------------------------------------------------------

		else:
			if not self.temp_mac_running:
				threading.Thread(target=self.cpu_temp_mac).start()

		return int(self.temp)

	def cpu_temp_mac(self):

		self.temp_mac_running=True
		while 1:
			cmd=path+"/os/mac/sensor/tempmonitor -l -a"
			temp_out=sh(cmd)
			try:
				heat_sink=temp_out.split("SMC CPU A HEAT SINK: ")[-1].split()[0]
				temp=int(heat_sink)
			except:
				try:
					proximity=temp_out.split("SMC CPU A PROXIMITY: ")[-1].split()[0]
					temp=int(proximity)
				except:temp=0

			self.temp=temp

			sleep(15)

	def ram_percent(self):
		if platform=="linux2":
			try:
				mem=fread("/proc/meminfo")
				total=int(mem.split("MemTotal:")[1].split("kB")[0])
				free=int(mem.split("MemFree:")[1].split("kB")[0])
				buffers=int(mem.split("Buffers:")[1].split("kB")[0])
				cached=int(mem.split("Cached:")[1].split("kB")[0])
				used=(total-free)-(buffers+cached)
				percent=int((used*100)/total) 
			except:None

			try:percent
			except:percent=0

			return percent
		else:
			return int(psutil.virtual_memory().percent)

	def ram_total(self):
		if platform=="linux2":
			return self.total_ram
		else:	
			return int(float(psutil.virtual_memory().total)/(1024**3))

	def ram_used(self):
		return round((float(self.ram_total())*self.ram_percent())/100,1)

resources=resources()

