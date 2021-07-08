cd $(dirname "$0")

nuke_plugins='/opt/Nuke12.1v2/plugins'
menu_py="$nuke_plugins/menu.py"

install()
{
    echo 'import nuke_vinarender #vinarender' >> $menu_py
    echo 'nuke.menu("Nuke").addCommand("Vina/VinaRender Submit", "nuke_vinarender.submit()", "Ctrl+R") #vinarender' >> $menu_py

    cp '../modules/nuke_vinarender.py' $nuke_plugins
    cp '../utils/python/util.py' "$nuke_plugins/vinarender_util.py"
}

uninstall()
{
    sed -i '/#vinarender/d' $menu_py

    rm "$nuke_plugins/nuke_vinarender.py"
    rm "$nuke_plugins/nuke_vinarender.pyc"
    rm "$nuke_plugins/vinarender_util.py"
    rm "$nuke_plugins/vinarender_util.pyc"
}

uninstall
install
