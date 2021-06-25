

install_dir="/c/Program Files/vinarender"


mkdir "$install_dir"
cd "$install_dir"


function download_qt5() {
    qt5_url='https://download.qt.io/official_releases/qt/5.12/5.12.11/qt-opensource-windows-x86-5.12.11.exe'

    vc_redist='https://download.visualstudio.microsoft.com/download/pr/76a91598-ca94-410b-b874-c7fa26e400da/91C21C93A88DD82E8AE429534DACBC7A4885198361EAE18D82920C714E328CF9/VC_redist.x86.exe'

    # start $qt5_url
    start $vc_redist
}

download_qt5
