thread=8

sh './scripts/svg_converter.sh'

sudo rm -rf /opt/vinarender/resources
sudo cp -rf ./resources /opt/vinarender

monitor_source="./source/monitor"
cd $monitor_source

rm ./release/vmonitor

qmake-qt5
make -j $thread

pkill -9 vmonitor

./release/vmonitor

