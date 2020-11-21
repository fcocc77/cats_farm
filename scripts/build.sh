thread=4

qmake-qt5
make -j $thread

rm ./bin/*
mkdir ./bin

# copia los ejecutables a la carpeta bin
cp ./source/manager/release/vmanager ./bin
cp ./source/server/release/vserver ./bin
cp ./source/monitor/release/vmonitor ./bin
cp ./source/logger/release/logger ./bin
cp ./source/submit/release/submit ./bin
cp ./source/videovina/release/videovina ./bin
