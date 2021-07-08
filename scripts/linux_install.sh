#!/usr/bin/env sh
cd "$(dirname "$0")"
path="$(dirname $(pwd))"

if [[ $EUID -ne 0 ]]; then
   echo "Execute with root: sudo sh install.sh"
   exit 1
fi

source ./nuke_module_install.sh

# ruta de instalacion
dst="/opt/vinarender"
app_data="/home/$SUDO_USER/.vinarender"

# IPs
manager_ip="$2"

# PORTs
manager_port=771
server_port=772

manager_start=$3
server_start=$4
logger_start=$5

compile() {
    folder=$path/source/$1
    cd $folder
    rm ./Makefile
    qmake-qt5
    make -j 8
    bin=$path/bin
    mkdir -p $bin
    mv "$folder/release/$2" $bin
}

install() {
    # Compilacion de todo
    compile server vserver
    compile manager vmanager
    compile monitor vmonitor
    compile submit submit
    compile videovina videovina
    compile logger logger

    # Creacion de servicios
    vserver="/etc/systemd/system/vserver.service"
    vmanager="/etc/systemd/system/vmanager.service"
    vlogger="/etc/systemd/system/vlogger.service"

    cp $path/services/vserver.service $vserver
    cp $path/services/vmanager.service $vmanager
    cp $path/services/vlogger.service $vlogger

    sed -i "s|{{port}}|$server_port|g" $vserver
    sed -i "s|{{port}}|$manager_port|g" $vmanager

    for service in $vserver $vmanager $vlogger; do
        sed -i "s|{{path}}|$dst|g" $service
    done

    systemctl daemon-reload

    # los servicios son muy estrictos asi que esto corrige el servicio, si este se modifico mal
    sed -i -e 's/\r//g' $path/services/service.sh

    mkdir $dst
    mkdir $dst/bin

    if [ ! -d "$app_data" ]; then
        mkdir "$app_data"
        cp "$path"/etc/* $app_data
    fi
    echo $app_data

    # copia el contenido necesario
    cp "$path/etc/settings.json" "$app_data/settings.json"
    cp -rf "$path/bin" $dst
    cp -rf "$path/resources" $dst
    cp -rf "$path/log" $dst
    cp -rf "$path/services" $dst
    cp -rf "$path/modules" $dst

    # guarda ruta de instalacion en etc del sistema operativo
    echo $dst >/etc/vinarender

    # guarda ip del manager y puertos en el settings
    settings="$app_data/settings.json"
    sed -i "s|{{server_port}}|$server_port|g" $settings
    sed -i "s|{{manager_port}}|$manager_port|g" $settings
    sed -i "s|{{manager_ip}}|$manager_ip|g" $settings

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

    # Acceso directo
    mv $dst/resources/app/vinarender.sh $dst/vinarender.sh

    echo "[Desktop Entry]
    Name=VinaRender Monitor
    Exec=sh $dst/vinarender.sh
    Icon=$dst/resources/icons/icon.png
    Categories=Graphics;2DGraphics;RasterGraphics;FLTK;
    Type=Application" >"/usr/share/applications/VinaRender.desktop"

    # modifica los permisos de los directorios etc y log para
    # que el vmonitor tenga acceso ya que se ejecuta en usuario.
    chown $SUDO_USER:$SUDO_USER -R "$app_data"
    chmod 777 -R "$dst/log"

    nuke_install $path
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

    nuke_uninstall $path
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
