# ruta de instalacion
dst="/opt/cats_farm"
# ------------------
# IP del manager
ip="192.168.1.77"
# ------------------
manager_start=true
server_start=true


compile() {
    folder=$path/code/$1
    cd $folder
    /opt/Qt5.11.3/5.11.3/gcc_64/bin/qmake
    make
    mv $folder/release/*.exe $path/bin/win 
}

install() {
    # Si no esta copiada la libreria qt5 la copia
    qt5="/opt/Qt5.11.3"
    if ! [[ -d $qt5 ]]; then
        tar -xzvf $path/qt/linux/Qt5.11.3.tar.gz -C "/opt"
    fi
    # -----------------------------

    # install rpms nesesarios
    yum -y install epel-release
    yum -y install http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
    yum -y install mesa-libGL-devel mesa-libGLU-devel
    yum -y install pulseaudio-libs-glib2.x86_64
    yum -y install ffmpeg
    yum -y install lm_sensors
    yum -y group install "Development Tools"
    yum -y install gcc-c++
    yum -y install sshpass
    yum -y install psmisc #fuser
    # ----------------------

    # Compilacion de todo
    compile server
    compile manager
    compile monitor
    compile submit
    # ----------------------

    echo $ip > $dst"/etc/manager_host"

    # Copia librerias necesarias en lib/bin
    qt="/opt/Qt5.11.3/5.11.3/gcc_64/lib"
    platforms="/opt/Qt5.11.3/5.11.3/gcc_64/plugins/platforms"

    mkdir -p $path/bin/linux/lib
    mkdir -p $path/bin/linux/plugins/platforms

    cp $qt/5libicudata.so.56 $qt/libicui18n.so.56 $qt/libicuuc.so.56 $qt/libQt5Core.so.5 $qt/libQt5DBus.so.5 $qt/libQt5Gui.so.5 $qt/libQt5Multimedia.so.5 $qt/libQt5Network.so.5 $qt/libQt5Widgets.so.5 $qt/libQt5XcbQpa.so.5 $path/bin/linux/lib

    cp $platforms/libqoffscreen.so $platforms/libqxcb.so $path/bin/linux/plugins/platforms
    # ------------------------------------------

    chmod 755 -R $dst

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
    cp -rf "$path/bin/linux" $dst/bin
    cp -rf "$path/etc" $dst
    cp -rf "$path/icons" $dst
    cp -rf "$path/log" $dst
    cp -rf "$path/os/win" $dst/os
    cp -rf "$path/sound" $dst
    cp -rf "$path/modules" $dst
    cp -rf "$path/theme" $dst
    # # ------------------------

    # inicializacion de servicios
    if server_start; then
        systemctl start cserver
        systemctl enable cserver
    fi

    if manager_start; then
        systemctl start cmanager
        systemctl enable cmanager
    fi
    # -----------------------------
}

uninstall() {
    systemctl stop cmanager
    systemctl stop cserver

    rm -rf $dst
}

uninstall
install


