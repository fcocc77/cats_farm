import os
import NatronEngine


def reload_nodes():
    # recarga todos los nodos, de videovina, con el comando 'Control-R'
    # para poder desarrollar los plugins sin recargar el natron.

    natron_tools = '/home/pancho/Documents/GitHub/vinarender/apis/natron/tools'
    plugins = natron_tools + '/plugins'

    # reinstala los nodos
    os.system('cd ' + natron_tools + ' && sh install.sh')
    # ---------------------

    # recarga todos los plugin que estan en la carpeta plugins
    for plugin in os.listdir(plugins):
        plugin = plugin.split('.')[0]

        reload(eval(plugin))
    # -------------------------

    print('Reload OK')


NatronGui.natron.addMenuCommand('videovina/Reload Nodes', 'reload_nodes',
                                QtCore.Qt.Key.Key_R, QtCore.Qt.KeyboardModifier.ControlModifier)
