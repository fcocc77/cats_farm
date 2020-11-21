include(../../config/config.pro)

TEMPLATE = app
TARGET = videovina
RC_ICONS = ../../icons/server.ico

CONFIG += console

QT += core gui multimedia widgets

INCLUDEPATH += ./
INCLUDEPATH += ../../utils/c++

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../utils/tcp.h \
			../../utils/c++/*.h \

SOURCES += 	./*.cc \
			../../utils/c++/*.cc \
