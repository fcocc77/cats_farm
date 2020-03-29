import os
import shutil
import filecmp
import NatronEngine


def reload_nodes():
    develop_plugins = '/home/pancho/Documents/GitHub/vinarender/apis/natron/tools/plugins'
    natron_plugins = '/opt/Natron2/Plugins/PyPlugs'

    # recarga todos los nodos, de videovina, con el comando 'Control-R'
    # para poder desarrollar los plugins sin recargar el natron.
    for plugin in os.listdir(develop_plugins):
        develop_plugin = develop_plugins + '/' + plugin
        natron_plugin = natron_plugins + '/' + plugin

        # si el plugin no es el mismo lo copia a la
        # carpeta de natron y lo recarga
        if not filecmp.cmp(develop_plugin, natron_plugin):
            shutil.copy(develop_plugin, natron_plugin)

            plugin = plugin.split('.')[0]
            reload(eval(plugin))
            print(plugin + ': has updated.')


NatronGui.natron.addMenuCommand('videovina/Reload Nodes', 'reload_nodes',
                                QtCore.Qt.Key.Key_R, QtCore.Qt.KeyboardModifier.ControlModifier)
