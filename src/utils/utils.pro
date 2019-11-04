TEMPLATE = app
TARGET = utils
CONFIG += console c++11 debug

QT += core gui multimedia widgets 
CONFIG -= app_bundle


HEADERS += 	util.hpp os.hpp video.hpp tcp.hpp threading.hpp

SOURCES += 	main.cpp util.cpp os.cpp video.cpp 