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
    compile logger
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
    $nssm install cserver $dst/bin/win/cserver.exe
    $nssm install cmanager $dst/bin/win/cmanager.exe
    $nssm install coreTemp $dst/os/win/core_temp/core_temp.exe
    # -----------------------------
    $nssm set "cmanager" ObjectName .\\$user $password
    $nssm set "cserver" ObjectName .\\$user $password

    # Inicializa servicios
    if $server_start; then
        sc start "cserver"
    else
        sc stop "cserver"
        sc config "cserver" start= disabled
    fi
    # -----------------------
    if $manager_start; then
        sc start "cmanager"
    else
        sc stop "cmanager"
        sc config "cmanager" start= disabled
    fi
    # -----------------------
    sc start "coreTemp"
    # -----------------------------

    # desbloquear puertos firewall
    netsh advfirewall firewall delete rule name="CatsFarm Ports:7000,7001"  # delete

    netsh advfirewall firewall add rule name="CatsFarm Ports:7000,7001" dir=in action=allow enable=yes profile=Any protocol=TCP localport=7000,7001
    netsh advfirewall firewall add rule name="CatsFarm Ports:7000,7001" dir=out action=allow enable=yes profile=Any protocol=TCP localport=7000,7001
    # ------------------------------------------------------------------------------------
}

uninstall() {
    nssm="$path/os/win/service/nssm.exe"
    $nssm stop "cserver"
    $nssm stop "cmanager"
    $nssm stop "coreTemp"

    $nssm remove "cserver" confirm
    $nssm remove "cmanager" confirm
    $nssm remove "coreTemp" confirm

    taskkill -f -im "cmonitor.exe"

    rm "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/CatsFarm Monitor.lnk"
    rm -rf $dst
}

uninstall
install