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
    nssm="$path/os/win/service/nssm.exe"
    $nssm install cServer $dst/bin/win/cServer.exe
    $nssm install cManager $dst/bin/win/cManager.exe
    $nssm install coreTemp $dst/os/win/core_temp/core_temp.exe
    # -----------------------------
    $nssm set "cManager" ObjectName .\\$user $password
    $nssm set "cServer" ObjectName .\\$user $password

    # Inicializa servicios
    sc start "cServer"
    sc start "cManager"
    sc start "coreTemp"
    # -----------------------------
}

uninstall() {
    nssm="$path/os/win/service/nssm.exe"
    $nssm stop "cServer"
    $nssm stop "cManager"
    $nssm stop "coreTemp"

    $nssm remove "cServer" confirm
    $nssm remove "cManager" confirm
    $nssm remove "coreTemp" confirm

    taskkill -f -im "cMonitor.exe"

    rm "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/CatsFarm Monitor.lnk"
    rm -rf $dst
}

uninstall
install


