thread=4

qmake-qt5
make -j $thread

rm ./bin/*

# copia los ejecutables a la carpeta bin
cp ./src/manager/release/vmanager ./bin
cp ./src/server/release/vserver ./bin
cp ./src/monitor/release/vmonitor ./bin
cp ./src/logger/release/logger ./bin
cp ./src/submit/release/submit ./bin
cp ./src/videovina/release/videovina ./bin
