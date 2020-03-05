import vina_to_natron
import NatronEngine
app = app1

path = vina_to_natron.path
vina_to_natron.app = app

# carga proyecto natron
app.loadProject(path + "/project/test.ntp")

vina_to_natron.main()

# conecta el nodo 'Control' al write creado
writer = app.createWriter(path + "/project/render/test.mov")
writer.connectInput(0, app.Control)
