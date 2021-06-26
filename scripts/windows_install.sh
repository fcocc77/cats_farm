cd "$(dirname "$0")"

qmake="/c/Qt/Qt5.12.11/5.12.11/mingw73_64/bin/qmake.exe"
make="/c/Qt/Qt5.12.11/Tools/mingw730_64/bin/mingw32-make.exe"

install_dir="/c/Program Files/vinarender"

manager_ip=$(ipconfig | awk -F":" '/IPv4/{print $2}')
manager_port=771
server_port=772

function download_qt5() {
    qt5_url='https://download.qt.io/official_releases/qt/5.12/5.12.11/qt-opensource-windows-x86-5.12.11.exe'
    vc_redist='https://download.visualstudio.microsoft.com/download/pr/76a91598-ca94-410b-b874-c7fa26e400da/91C21C93A88DD82E8AE429534DACBC7A4885198361EAE18D82920C714E328CF9/VC_redist.x86.exe'

    start $qt5_url
    start $vc_redist
}

function compile() {
    folder="$install_dir/source/$1"
    cd "$folder"
    $qmake
    $make -j 8
    bin="$install_dir/bin"
    mkdir -p "$bin"
    mv "$folder/release/$2.exe" "$bin"
}

function install(){
    if [ ! -d "$install_dir" ]; then
        mkdir "$install_dir"
    fi

    cp -rf ../etc "$install_dir"
    cp -rf ../config "$install_dir"
    cp -rf ../libs "$install_dir"
    cp -rf ../log "$install_dir"
    cp -rf ../modules "$install_dir"
    cp -rf ../resources "$install_dir"
    cp -rf ../services "$install_dir"
    cp -rf ../source "$install_dir"
    cp -rf ../utils "$install_dir"
    cp -rf ../win "$install_dir"

    compile server vserver
    compile manager vmanager
    compile monitor vmonitor
    compile submit submit

    # guarda ip del manager y puertos en el settings
    settings="$install_dir/etc/settings.json"
    sed -i "s|{{server_port}}|$server_port|g" "$settings"
    sed -i "s|{{manager_port}}|$manager_port|g" "$settings"
    sed -i "s|{{manager_ip}}|$manager_ip|g" "$settings"

    # cambia ruta de linux por la de windows
    sed -i "s|/opt/vinarender|C:/Program Files/vinarender|g" "$install_dir/resources/css/style.css"

    # service
	nssm="$install_dir/win/service/nssm.exe"
    "$nssm" install "VinaRender Server" "$install_dir/bin/vserver.exe"
    "$nssm" install "VinaRender Manager" "$install_dir/bin/vmanager.exe"

    "$nssm" set "VinaRender Server" ObjectName ".\\$user" "$password"
    "$nssm" set "VinaRender Manager" ObjectName ".\\$user" "$password"

    sc start "VinaRender Server"
    sc start "VinaRender Manager"

	# CoreTemp service
    "$nssm" install "VinaRender CoreTemp" "$install_dir/win/sensor/core_temp.exe"
    "$nssm" start "VinaRender CoreTemp"

	# Firewall
    netsh advfirewall firewall delete rule name="VinaRender Ports:$manager_port,$server_port"

    for direction in in out
    do
        netsh advfirewall firewall add rule name="VinaRender Ports:$manager_port,$server_port" \
            dir=$direction action=allow enable=yes profile=Any protocol=TCP localport=$manager_port,$server_port
    done
}

function uninstall(){
	nssm="$install_dir/win/service/nssm.exe"

    "$nssm" stop "VinaRender Server"
    "$nssm" remove "VinaRender Server" confirm

    "$nssm" stop "VinaRender Manager"
    "$nssm" remove "VinaRender Manager" confirm

    "$nssm" stop "VinaRender CoreTemp"
    "$nssm" remove "VinaRender CoreTemp" confirm

    taskkill -f -im "vmonitor.exe"

    rm -rf "$install_dir"
}

if [ ! -d "C:/Qt" ]; then
    download_qt5
else
    echo "UserName: "
    read user
    echo "Password: "
    read -s password

    uninstall
    install
fi


