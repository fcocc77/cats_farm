#!/usr/bin/env sh
path="$(
    cd "$(dirname "$0")"
    pwd -P
)"

# ruta de instalacion
dst="/opt/vinarender"
# ------------------
# IPs
manager_ip="192.168.10.10"
# ------------------
# PORTs
manager_port=771
server_port=772
# -----------------
manager_start=false
server_start=true
logger_start=false

compile() {
    folder=$path/src/$1
    cd $folder
    qmake-qt5
    make
    bin=$path/bin
    mkdir -p $bin
    mv $folder/$2 $bin
}

nuke() {
    nuke_path="/usr/local/Nuke10.5v8"
    plugins=$nuke_path"/plugins"
    menu_py=$plugins"/menu.py"

    line1="import nukeVinaRender"
    line2="menu_bar = nuke.menu('Nuke')"
    line3="menu_bar.addCommand('J_Script/SendToVinaRender...', 'nukeVinaRender.VinaRenderSend()', 'Ctrl+r')"

    if [ $1 == install ]; then
        # copia modulos al directorio de plugins de nuke
        cp $dst"/modules/nuke/util.py" $plugins
        cp $dst"/modules/nuke/nukeVinaRender.py" $plugins
        # ------------------------------

        # Agrega accesos directo a vinarender
        echo $line1 >>$menu_py
        echo $line2 >>$menu_py
        echo $line3 >>$menu_py
        # ---------------
    fi
    if [ $1 == uninstall ]; then
        # borra los modulos de nuke
        rm $plugins"/util.py"
        rm $plugins"/nukeVinaRender.py"
        # -----------------
        # borra todas las lineas de vinarender
        sed -i "/$line1/d" $menu_py
        sed -i "/$line2/d" $menu_py
        sed -i "/SendToVinaRender/d" $menu_py
        # -----------------------
    fi
}

install() {
    # Instalacion de Dependencias
    yum -y install epel-release http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
    yum -y install \
        qt5-qtbase \
        qt5-qtbase-devel \
        qt5-qtmultimedia.x86_64 \
        qt5-qtmultimedia-devel.x86_64 \
        qt5-qtsvg.x86_64 \
        qt5-qtsvg-devel.x86_64 \
        mesa-libGL-devel \
        mesa-libGLU-devel \
        pulseaudio-libs-glib2 \
        ffmpeg \
        lm_sensors \
        gcc-c++ \
        sshpass \
        net-tools \
        psmisc #fuser

    yum -y group install "Development Tools"
    # ----------------------

    # Compilacion de todo
    compile server vserver
    compile manager vmanager
    compile monitor vmonitor
    compile submit submit
    compile videovina videovina
    compile logger logger
    # ----------------------

    # Creacion de servicios
    vserver="/etc/systemd/system/vserver.service"
    vmanager="/etc/systemd/system/vmanager.service"
    vlogger="/etc/systemd/system/vlogger.service"

    cp $path/os/linux/init/vserver.service $vserver
    cp $path/os/linux/init/vmanager.service $vmanager
    cp $path/os/linux/init/vlogger.service $vlogger

    sed -i "s|{{port}}|$server_port|g" $vserver
    sed -i "s|{{port}}|$manager_port|g" $vmanager

    for service in $vserver $vmanager $vlogger; do
        sed -i "s|{{path}}|$dst|g" $service
    done

    systemctl daemon-reload
    # -----------------------------

    # los servicios son muy estrictos asi que esto corrige el servicio, si este se modifico mal
    sed -i -e 's/\r//g' $path/os/linux/init/service.sh
    # --------------------------------------------------------------------------------

    mkdir $dst
    mkdir $dst/os
    mkdir $dst/bin

    # # copia el contenido necesario
    cp -rf "$path/bin" $dst
    cp -rf "$path/etc" $dst
    cp -rf "$path/icons" $dst
    cp -rf "$path/log" $dst
    cp -rf "$path/src" $dst
    cp -rf "$path/os/linux" $dst/os
    cp -rf "$path/modules" $dst
    # # ------------------------

    # guarda ruta de instalacion en etc del sistema operativo
    echo $dst >/etc/vinarender
    # -----------------------

    # guarda ip del manager y puertos en el settings
    settings=$dst"/etc/settings.json"
    sed -i "s|{{server_port}}|$server_port|g" $settings
    sed -i "s|{{manager_port}}|$manager_port|g" $settings
    sed -i "s|{{manager_ip}}|$manager_ip|g" $settings
    # -------------------------------------------

    chmod 755 -R $dst

    # inicializacion de servicios
    if $server_start; then
        systemctl start vserver
        systemctl enable vserver
    fi

    if $manager_start; then
        systemctl start vmanager
        systemctl enable vmanager
    fi
    if $logger_start; then
        systemctl start vlogger
        systemctl enable vlogger
    fi
    # -----------------------------

    # Acceso directo
    echo "[Desktop Entry]
    Name=VinaRender Monitor
    Exec=$dst/bin/vmonitor
    Icon=$dst/icons/icon.png
    Categories=Graphics;2DGraphics;RasterGraphics;FLTK;
    Type=Application" >"/usr/share/applications/VinaRender.desktop"
    # ---------------

    nuke install
}

uninstall() {
    systemctl stop vmanager
    systemctl stop vserver
    systemctl stop vlogger
    pkill -9 vmonitor

    rm /etc/systemd/system/vmanager.service
    rm /etc/systemd/system/vserver.service
    rm /etc/systemd/system/vlogger.service

    rm "/usr/share/applications/VinaRender.desktop"
    rm -rf $dst

    nuke uninstall
}

if [ $1 == install ]; then
    install
fi
if [ $1 == uninstall ]; then
    uninstall
fi
if [ $1 == reinstall ]; then
    uninstall
    install
fi
