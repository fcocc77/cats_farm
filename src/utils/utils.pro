TEMPLATE = app
TARGET = utils
CONFIG += console c++11 debug

QT += core gui multimedia widgets 
CONFIG -= app_bundle


HEADERS += 	util.h os.h video.h tcp.h threading.h

SOURCES += 	main.cpp util.cpp os.cpp video.cpp 