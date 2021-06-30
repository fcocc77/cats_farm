cd "$(dirname "$0")"
path="$(dirname "$(pwd)")"

thread=8
bin='/opt/vinarender/bin'

update_monitor() {
    # Icons update
    sh "$path/scripts/svg_converter.sh"
    sudo rm -rf /opt/vinarender/resources
    sudo cp -rf "$path/resources" /opt/vinarender

    pkill -9 vmonitor

    cd "$path/source/monitor"

    rm './release/vmonitor'
    qmake-qt5
    make -j $thread

    sudo rm "$bin/vmonitor"
    sudo cp './release/vmonitor' "$bin/vmonitor"

    mate-terminal -e "sh -c \"$bin/vmonitor\"" --geometry 70x20+0+475
}

update_manager() {
    sudo systemctl stop vmanager
    sudo pkill -9 vmanager

    cd "$path/source/manager"

    rm './release/vmanager'
    qmake-qt5
    make -j $thread

    sudo rm "$bin/vmanager"
    sudo cp './release/vmanager' "$bin/vmanager"

    mate-terminal -e "sh -c \"echo vfx | sudo -S /opt/vinarender/bin/vmanager\"" --geometry 70x10+0+0
}

update_server() {
    sudo systemctl stop vserver
    sudo pkill -9 vserver

    cd "$path/source/server"

    rm './release/vserver'
    qmake-qt5
    make -j $thread

    sudo rm "$bin/vserver"
    sudo cp './release/vserver' "$bin/vserver"

    mate-terminal -e "sh -c \"echo vfx | sudo -S /opt/vinarender/bin/vserver\"" --geometry 70x10+0+250
}

if [ $1 == 'manager' ]; then
    update_manager
fi

if [ $1 == 'server' ]; then
    update_server
fi

if [ $1 == 'monitor' ]; then
    update_monitor
fi
