from sys import argv
import NatronEngine

app = app1

project = argv[5] + '.ntp'
node = argv[6]
output = argv[7]
ext = output.split('.')[-1]

app.loadProject(project)

node = eval('app.' + node)

writer = app.createWriter(output)

# el tamanio del render es igual al del nodo
writer.getParam('formatType').setValue(0)
# ---------------------

# 0 = ap4h Apple ProRess 4444
# 1 = apch Apple ProRess 422 HQ
# 2 = apcn Apple ProRess 422
if ext == 'mov':
    writer.getParam('codec').setValue(2)
# ------------------

writer.connectInput(0, node)