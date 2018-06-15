
# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = tcpclient

CONFIG += console c++14 

QT += core gui multimedia widgets

HEADERS += 	mytcpsocket.h 


SOURCES += 	main.cpp  mytcpsocket.cpp


