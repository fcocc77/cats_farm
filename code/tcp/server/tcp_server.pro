
# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = tcpserver

CONFIG += console c++14 

QT += core gui multimedia widgets

HEADERS += 	mytcpserver.h 

SOURCES += 	main.cpp mytcpserver.cpp


