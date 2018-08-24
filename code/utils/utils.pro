TEMPLATE = app
TARGET = utils
CONFIG += console c++11

QT += core gui multimedia widgets

HEADERS += 	util.h video.h os.h json.h tcp.h

SOURCES += 	main.cpp util.cpp video.cpp os.cpp 