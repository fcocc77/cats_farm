path="$( cd "$(dirname "$0")" ; pwd -P )"

# ruta de instalacion
dst="/opt/cats_farm"
# ------------------
# IP del manager
ip="192.168.0.18"
# ------------------
manager_start=true
server_start=true


compile() {
    folder=$path/src/$1
    cd $folder
    qmake-qt5
    make
    bin=$path/bin
    mkdir -p $bin
    mv $folder/$2 $bin 
}

install() {
    # Instalacion de Dependencias
    yum -y install \
    http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm \
    epel-release \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtmultimedia.x86_64 \
    qt5-qtmultimedia-devel.x86_64 \
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
    compile server cserver
    compile manager cmanager
    compile monitor cmonitor
    compile submit submit 
    # ----------------------

    echo $ip > $path"/etc/manager_host"

    # Creacion de servicios
    cp $path/os/linux/init/cserver.service /etc/systemd/system
    cp $path/os/linux/init/cmanager.service /etc/systemd/system
    systemctl daemon-reload
    # -----------------------------

    # los servicios son muy estrictos asi esto corrige el servicio si de modifico mal
    sed -i -e 's/\r//g' $path/os/linux/init/cserver.sh
    sed -i -e 's/\r//g' $path/os/linux/init/cmanager.sh
    # --------------------------------------------------------------------------------

    mkdir $dst
    mkdir $dst/os
    mkdir $dst/bin

    # # copia el contenido necesario
    cp -rf "$path/bin" $dst
    cp -rf "$path/etc" $dst
    cp -rf "$path/icons" $dst
    cp -rf "$path/log" $dst
    cp -rf "$path/os/linux" $dst/os
    cp -rf "$path/sound" $dst
    cp -rf "$path/modules" $dst
    cp -rf "$path/theme" $dst
    # # ------------------------

    chmod 755 -R $dst

    # inicializacion de servicios
    if $server_start; then
        systemctl start cserver
        systemctl enable cserver
    fi

    if $manager_start; then
        systemctl start cmanager
        systemctl enable cmanager
    fi
    # -----------------------------
}

uninstall() {
    systemctl stop cmanager
    systemctl stop cserver

    rm /etc/systemd/system/cmanager.service
    rm /etc/systemd/system/cserver.service

    rm -rf $dst
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