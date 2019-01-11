import sys
from sys import platform
import nuke

# detecta el sistema operativo
if platform == "win32":
    system = "Windows"
    path = "C:/cats_farm"
    bin = path + "/bin/win"
elif platform == "darwin":
    system = "Mac"
    path = "/usr/local/cats_farm"
    bin = path + "/bin/mac"
else:
    system = "Linux"
    path = "/opt/cats_farm"
    bin = path + "/bin/linux"
# -------------------------------------------------------

sys.path.insert(0, path + '/modules')
from util import *

class catsFarmSend():
    def __init__(self):
        self.write = nuke.selectedNode()
        checkOK = self.checkProject()

        if checkOK:
            if self.write.Class() == "Write":
                self.general_data()
            else:
                nuke.message("No Write type.")

            if not self.write:
                nuke.message("You have to select the Write Node.")

    def general_data(self):

        # Datos Generales
        writFile = self.write["file"].value()
        render = self.write['name'].value()

        writeFirstFrame = str(self.write.firstFrame())
        writeLastFrame = str(self.write.lastFrame())

        script = nuke.root().knob('name').value()
        scriptName = script.split("/")[-1].split(".")[0]
        # ------------------------------------------------

        # Crea Panel y botones
        p = nuke.Panel('Send To Backburner')
        p.addSingleLineInput('Job Name', scriptName)

        p.addSingleLineInput('Frame Range', writeFirstFrame+"-"+writeLastFrame)
        p.addSingleLineInput('Task Size', '10')

        p.addEnumerationPulldown(
            'Priority', "Normal Very\\ Low Low High Very\\ High")

        # posiciona los grupos con Nuke Name al inicio
        preferences = jread(path+"/etc/preferences_s.json")
        nukeGroups = ["NUKE", "nuke", "Nuke"]
        serverGroups = []
        for s in preferences["groups"]:
            if not s in nukeGroups:
                serverGroups.append(s)
            else:
                serverGroups.insert(0, s)
        # -----------------------------------------------

        if serverGroups:
            serverGroup = ""
        else:
            serverGroup = "none"

        for s in serverGroups:
            serverGroup = serverGroup+" "+s
        p.addEnumerationPulldown('Server Group', serverGroup)

        p.addEnumerationPulldown('Instances', "1 2 3 4")

        p.addBooleanCheckBox('Manually Start Job', False)

        p.addButton('Cancel')
        p.addButton('Render')

        self.result = p.show()
        # -------------------------------------------

        # Obtener el resultado del panel
        jobName = str(p.value("Job Name"))

        firstFrame = p.value("Frame Range").split("-")[0]
        lastFrame = p.value("Frame Range").split("-")[1]
        taskSize = p.value("Task Size")
        priority = str(p.value("Priority"))

        if priority == "Normal":
            priority = "2"
        if priority == "Very High":
            priority = "0"
        if priority == "High":
            priority = "1"
        if priority == "Low":
            priority = "3"
        if priority == "Very Low":
            priority = "4"

        serverGroup = str(p.value("Server Group"))
        instances = str(p.value("Instances"))

        suspend = p.value("Manually Start Job")
        # ---------------------------------------------------

        # Guarda projecto actual
        nuke.scriptSave(nuke.toNode("root").name())
        # --------------------------------------------

        if self.result:

            nameFile = writFile.split("/")[-1]
            namePath = writFile.replace(nameFile, "")
            name_base_pad = nameFile.split(".")[-2]
            pad = name_base_pad.split("_")[-1]
            extension = writFile.split(".")[-1]

            scriptBase = script.replace((script.split("/")[-1]), "")
            scriptName = script.split("/")[-1].split(".")[0]

            # si el archivo es un mov le agrega padding
            if extension == "mov":
                nameBase = nameFile.split(".")[-2]
                namePadding = namePath+nameBase+"/"+nameBase+"_###########."+extension
            else:
                nameBase = name_base_pad.replace("_"+pad, "")
                namePadding = namePath+nameBase+"/"+nameBase+"_"+pad+"."+extension
        # ----------------------------------------------------------------------------------------------

        # Guarda version de render
            script_save = scriptBase+"."+scriptName+"_render_01.nk"

            for n in range(1, 1000):
                check_file = os.path.isfile(script_save)

                if n < 10:
                    n = "0"+str(n)
                else:
                    n = str(n)

                if check_file:
                    script_save = scriptBase+"."+scriptName+"_render_"+n+".nk"
                else:
                    break
            # ---------------------------------------------------------------------------------------------------

            # lee projecto y remplaza las ruta de exportacion
            f = open(script, "r")
            r = f.read()
            writFile = r.split(
                render)[-2].split("file")[-2].split("\n")[0].strip()
            info = r.replace(writFile, '"'+namePadding+'"')
            f.close
            # --------------------------------------------
            f = open(script_save, "w")
            f.write(info)
            f.close
            # -------------------------------------------

            if suspend:
                suspend = '1'
            else:
                suspend = '0'

            serverGroup = serverGroup.replace("none", "None")
            # ---------------------------------

            cmd = (bin + "/submit.exe " +
                   "-jobName " + jobName +
                   " -server " + "none" +
                   " -serverGroup " + serverGroup +
                   " -firstFrame " + firstFrame +
                   " -lastFrame " + lastFrame +
                   " -taskSize " + taskSize +
                   " -priority " + priority +
                   " -suspend " + suspend +
                   " -comment " + "Nuke Render" +
                   " -software " + "Nuke" +
                   " -project " + '"' + script_save + '"' +
                   " -extra " + writFile + 
                   " -system " + system +
                   " -instances " + instances +
                   " -render " + render)

            os.system(cmd)

            nuke.message("Job Sended.")

    def checkProject(self):

        path_list = jread(path + "/etc/preferences_s.json")["paths"]["system"]
        rutas = []
        for r in path_list:
            if os.path.isdir(r):
                rutas.append(r)
        if not rutas:
            rutas.append("#none#")

        failedNodes = ""
        desconectNodes = ""

        checkOK = True

        nuke.selectConnectedNodes()

        for node in nuke.selectedNodes():
            name = node.name()
            try:
                file = node["file"].getText()
            except:
                file = None

            if file:
                dirname = os.path.dirname(file)
                rootScript = "[file dirname [value root.name]]"

                if rootScript in file:
                    None
                elif not os.path.isdir(dirname):
                    desconectNodes = desconectNodes+name+"="+file+"\n"
                    checkOK = False
                else:
                    f = False
                    for ruta in rutas:
                        if ruta in file:
                            f = True
                    if not f:
                        checkOK = False
                        failedNodes = failedNodes+name+"="+file+"\n"

        # deseleccionar todo
        for n in nuke.allNodes():
            n.knob("selected").setValue(False)
        # ------------------------------------------

        if not checkOK:
            if failedNodes:
                line1 = "These files are on your PC:"
            else:
                line1 = ""

            if desconectNodes:
                line2 = "These files are disconnected:"
            else:
                line2 = ""

            message = line1+"\n\n" + failedNodes + "\n\n"+line2+"\n\n" + desconectNodes
            nuke.message(message)

        return checkOK
