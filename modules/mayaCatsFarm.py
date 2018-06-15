import socket
import os
import maya.cmds as cmds
import sys

sys.path.insert(0,'/opt/CatsFarm/code')

from util import *

managerHostFile="/opt/CatsFarm/setting/manager_host"

# ip del catsfarm manager
managerHost=read(managerHostFile,"manager_ip").read
#--------------------------

class catsFarmSend():

	def __init__(self):

		self.catPanel()

	def catPanel(self):

		self.window = cmds.window('Cats Farm')

		cmds.rowColumnLayout( numberOfColumns=2, columnAttach=(1, 'right', 0), columnWidth=[(1, 100), (2, 250)] )

		fileScene = cmds.file(q=True, l=True)[0]
		name = fileScene.split("/")[-1].split(".")[0]

		cmds.text( label='' )
		cmds.text( label='' )

		cmds.text( label='Job Name: ' )
		self.jobName = cmds.textField(text=name)

		cmds.text( label='Frame Range: ' )
		self.frameRange = cmds.textField(text="1-100")

		cmds.text( label='Task Size: ' )
		self.taskSize = cmds.textField(text="10")

		cmds.text( label='Priority: ' )
		self.priority = cmds.textField(text="50")

		cmds.text( label='' )
		cmds.separator( height=30, style='in' )

		cmds.text( label='Server: ')
		self.server = cmds.optionMenu()
		cmds.menuItem( label="none" )
		for g in getInfo().server:
			cmds.menuItem( label=g )

		cmds.text( label='Server Group: ')
		self.serverGroup = cmds.optionMenu()
		cmds.menuItem( label="none" )
		for g in getInfo().serverGroup:
			cmds.menuItem( label=g )

		cmds.text( label='' )
		cmds.separator( height=30, style='in' )

		cmds.text( label='Suspended: ')
		self.suspend = cmds.checkBox(label='')

		cmds.text( label='' )
		cmds.separator( height=30, style='in' )

		cmds.text( label='' )
		cmds.rowColumnLayout( numberOfColumns=3, columnAttach=(1, 'right', 0), columnWidth=[(1, 100), (1, 100)] )

		cancelButton = cmds.button( label='Cancel', c=self.cancel )
		cmds.text( label=' ' )

		sendButton = cmds.button( label='Send To CatsFarm', c=self.sendTo )

		cmds.showWindow(self.window)

	def sendTo(self,*args):

		jobName=cmds.textField( self.jobName,q=True,text=True )
		frameRange=cmds.textField( self.frameRange,q=True,text=True )

		firstFrame=frameRange.split("-")[0]
		lastFrame=frameRange.split("-")[1]

		taskSize=cmds.textField( self.taskSize,q=True,text=True )
		priority=cmds.textField( self.priority,q=True,text=True )

		suspend=str(cmds.checkBox(self.suspend,q=True,v=True ))

		server=cmds.optionMenu(self.server,q=True,v=True)
		serverGroup= cmds.optionMenu(self.serverGroup,q=True,v=True)

		fileScene = cmds.file(q=True, l=True)[0]
		nameScene = fileScene.split("/")[-1]
		dirScene=fileScene.strip(nameScene)[:-1]

		# Encuentra el Projecto a partir de la escena
		for r in range(1,10):
			isScene=0
			for l in os.listdir(dirScene):
				if l =="workspace.mel":
					isScene=1
					break
			if isScene:
				break

			lastWord= (dirScene.split("/")[-1])
			dirScene= dirScene.strip(lastWord)[:-1]
		#----------------------------------------------

		dirProject=dirScene

		comment="Maya"
		software="Maya"

		project=" -r file -s %st -e %en -proj "+dirProject+" "+fileScene

		# crea lista de texto facil para poder enviar por tcp.
		submit_info=[jobName, server, serverGroup, firstFrame, lastFrame, taskSize, priority, suspend, comment, software, project, "extra", system().read, "1"]

		tcpClient(host=managerHost, port=7770, loop=0, sleep=1, background=0, pks=submit_info)
		#------------------------------------------------------

		cmds.deleteUI(self.window)

	def cancel(self):
		cmds.deleteUI(self.window)

catsFarmSend = catsFarmSend()

