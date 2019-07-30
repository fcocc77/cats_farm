
path="$( cd "$(dirname "$0")" ; pwd -P )"

# ruta de instalacion
dst="C:/cats_farm"
# ------------------
# IP del manager
ip="192.168.1.77"
# ------------------
manager_start=true
server_start=true
# Usuario y clave para los servicio
user=pancho
password=vfx
# --------------------

compile() {
    folder=$path/code/$1
    cd $folder
    qmake
    mingw32-make
    mv $folder/release/*.exe $path/bin/win 
}

install() {
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
    cp -rf "$path/bin/win" $dst/bin
    cp -rf "$path/etc" $dst
    cp -rf "$path/icons" $dst
    cp -rf "$path/log" $dst
    cp -rf "$path/os/win" $dst/os
    cp -rf "$path/sound" $dst
    cp -rf "$path/modules" $dst
    cp -rf "$path/theme" $dst
    # # ------------------------

    echo $ip > $dst"/etc/manager_host"

    # copia accecos directo
    cp "$path/os/win/link/CatsFarm Monitor.lnk" "C:/ProgramData/Microsoft/Windows/Start Menu/Programs"
    # -----------------

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


