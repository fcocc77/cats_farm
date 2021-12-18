
nuke_install()
{
    cd $1

    for version in $(ls '/opt' | grep Nuke); do

        nuke_plugins="/opt/$version/plugins"
        menu_py="$nuke_plugins/menu.py"

        echo 'import nuke_vinarender #vinarender' >> $menu_py
        echo 'nuke.menu("Nuke").addCommand("Vina/VinaRender Submit", "nuke_vinarender.submit()", "Ctrl+R") #vinarender' >> $menu_py

        cp './modules/nuke_vinarender.py' $nuke_plugins
        cp './python_util/util.py' "$nuke_plugins/vinarender_util.py"

    done
}

nuke_uninstall()
{
    for version in $(ls '/opt' | grep Nuke); do

        nuke_plugins="/opt/$version/plugins"
        menu_py="$nuke_plugins/menu.py"

        sed -i '/#vinarender/d' $menu_py

        rm "$nuke_plugins/nuke_vinarender.py"
        rm "$nuke_plugins/nuke_vinarender.pyc"
        rm "$nuke_plugins/vinarender_util.py"
        rm "$nuke_plugins/vinarender_util.pyc"

    done
}
