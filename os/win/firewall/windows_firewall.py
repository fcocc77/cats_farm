import os


def firewall(name="none", action="block", program="Any", profile="Any", protocol="Any", localport="Any"):

    apps = []
    if os.path.isdir(program):
        for root, dir, file in os.walk(program):
            for f in file:
                ext = f.split(".")[-1]
                if ext == "exe":
                    ruta = (root+"/"+f).replace("/", "\\")
                    apps.append((f, ruta))
    else:
        apps.append((name+" Ports:"+localport, program))

    if apps:
        for name, app in apps:
            for direction in ["in", "out"]:
                if app == "Any":
                    appss = ""
                else:
                    appss = ' program="'+app+'"'

                if localport == "Any":
                    localports = ""
                else:
                    localports = " localport="+localport

                rule = 'netsh advfirewall firewall add rule name="'+name+'" dir='+direction+' action=' + \
                    action+appss+' enable=yes  profile='+profile+' protocol='+protocol+localports

                os.system(rule)


# Elimina todas las reglas
delete_all_rules = "netsh advfirewall firewall delete rule name=all"
os.system(delete_all_rules)
# -------------------------------------------------------

firewall(action="block", program="C:/Program Files/Autodesk")

firewall(action="block", program="C:/Program Files/Common Files/Autodesk Shared")
firewall(action="block", program="C:/Program Files/Common Files/Macrovision Shared")
firewall(action="block",
         program="C:/Program Files (x86)/Common Files/Microsoft Shared")
firewall(action="block", program="C:/ProgramData/Autodesk")
firewall(action="block", program="C:/Program Files/solidangle")
firewall(action="block", program="C:/Program Files/rlm")
firewall(action="block", program="C:/Program Files/Nuke9.0v9")
firewall(action="block", program="C:/Program Files/Nuke10.0v4")
firewall(action="block", program="C:/Program Files/Nuke10.5v4")
firewall(action="block", program="C:/Program Files/Side Effects Software")
firewall(action="block", program="C:/Program Files/Adobe")
firewall(action="block", program='C:/Program Files/Common Files/Adobe')
firewall(action="block", program="C:/Program Files/MAXON")
firewall(action="block", program="C:/Program Files/Mari3.3v1")

firewall(name="VinaRender", action="allow",
         protocol="TCP", localport="7773,7774")

# Hbilita Firewall
enable_firewall = "netsh advfirewall set allprofiles state on"
os.system(enable_firewall)
# -------------------------------------
