thread=4

monitor_source="./source/monitor"

cd $monitor_source
qmake-qt5
make -j $thread

pkill -9 vmonitor

sudo cp ../../resources/css/style.css '/opt/vinarender/resources/css/style.css'

./release/vmonitor

