#!/usr/bin/env sh
path="$(
    cd "$(dirname "$0")"
    pwd -P
)"

# ruta de instalacion
dst="/opt/vinarender"
# ------------------
# IP del manager
ip="192.168.1.77"
# ------------------
manager_start=true
server_start=true
logger_start=true

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
    line2="nuke.menu("Nuke")"
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

    echo $ip >$path"/etc/manager_host"

    # Creacion de servicios
    cp $path/os/linux/init/vserver.service /etc/systemd/system
    cp $path/os/linux/init/vmanager.service /etc/systemd/system
    cp $path/os/linux/init/vlogger.service /etc/systemd/system

    sed -i "s|{{path}}|$dst|g" /etc/systemd/system/vserver.service
    sed -i "s|{{path}}|$dst|g" /etc/systemd/system/vmanager.service
    sed -i "s|{{path}}|$dst|g" /etc/systemd/system/vlogger.service

    systemctl daemon-reload
    # -----------------------------

    # los servicios son muy estrictos asi esto corrige el servicio si de modifico mal
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
