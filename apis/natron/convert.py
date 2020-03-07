import vina_to_natron
import NatronEngine

app = app1

path = vina_to_natron.path
vina_to_natron.app = app

# carga proyecto natron
app.loadProject(path + "/project/test_02.ntp")

vina_to_natron.main()

app.saveProjectAs(path + "/project/render.ntp")
