cd "$(dirname "$0")"
path="$(dirname "$(pwd)")"

qmake="/c/Qt/Qt5.12.11/5.12.11/mingw73_64/bin/qmake.exe"
make="/c/Qt/Qt5.12.11/Tools/mingw730_64/bin/mingw32-make.exe"

install_dir="/c/Program Files/vinarender"
app_data="$HOME/AppData/Local/vinarender"

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
    cd "$path/source/$1"

    $qmake
    $make -j 8
    mv "./release/$2.exe" "$path/bin"

    rm ./MakeFile
    rm ./*.Debug
    rm ./*.Release
    rm ./*.rc
    rm -rf ./debug
    rm -rf ./release
}

function build()
{
    bin="$path/bin"

    if [ ! -d "$bin" ]; then
        mkdir "$bin"
    fi

    compiled=true

    if [[ (! -d "C:/Qt") && (! -f "$bin/vmanager.exe") ]]; then
        download_qt5
    fi

    if [ ! -f "$bin/submit.exe" ]; then
        compile submit submit
        compiled=false
    fi

    if [ ! -f "$bin/vserver.exe" ]; then
        compile server vserver
    fi

    if [ ! -f "$bin/vmanager.exe" ]; then
        compile manager vmanager
    fi

    if [ ! -f "$bin/vmonitor.exe" ]; then
        compile monitor vmonitor
    fi

    if [ $compiled == false ]; then
        windeployqt "$bin"
    fi
}

function install(){
    if [ ! -d "$install_dir" ]; then
        mkdir "$install_dir"
    fi

    if [ ! -d "$app_data" ]; then
        mkdir "$app_data"
        cp -rf "$path"/etc/* "$app_data"
    fi

    cp "$path/etc/settings.json" "$app_data/settings.json"
    cp -rf $path/log "$install_dir"
    cp -rf $path/modules "$install_dir"
    cp -rf $path/resources "$install_dir"
    cp -rf $path/win "$install_dir"
    cp -rf $path/bin "$install_dir"

    # guarda ip del manager y puertos en el settings
    settings="$app_data/settings.json"
    sed -i "s|{{server_port}}|$server_port|g" "$settings"
    sed -i "s|{{manager_port}}|$manager_port|g" "$settings"
    sed -i "s|{{manager_ip}}|$manager_ip|g" "$settings"

    # cambia ruta de linux por la de windows
    sed -i "s|/opt/vinarender|C:/Program Files/vinarender|g" "$install_dir/resources/css/style.css"

    # link
    cmd //c mklink "$HOME/Desktop/VinaRender Monitor" "$install_dir/bin/vmonitor.exe"

    # service
	nssm="$install_dir/win/service/nssm.exe"
    "$nssm" install "VinaRender Server" "$install_dir/bin/vserver.exe"
    "$nssm" install "VinaRender Manager" "$install_dir/bin/vmanager.exe"

    "$nssm" set "VinaRender Server" ObjectName ".\\$USERNAME" "$password"
    "$nssm" set "VinaRender Manager" ObjectName ".\\$USERNAME" "$password"

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
    rm "$HOME/Desktop/VinaRender Monitor"
}

echo Password of \"$USERNAME\":
read -s password

echo -e "\nUninstalling ..."
uninstall &>/dev/null

echo 'Building ...'
build &>/dev/null

echo 'Installing ...'
install &>/dev/null

echo -e "\n// Installation completed //"


