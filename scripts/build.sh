thread=8

monitor_source="./source/monitor"

cd $monitor_source
qmake-qt5
make -j $thread

pkill -9 vmonitor

./release/vmonitor

