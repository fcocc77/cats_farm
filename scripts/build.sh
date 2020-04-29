cd src/monitor

thread=4

pkill -9 "vmonitor"

qmake-qt5
make -j $thread

./release/vmonitor
