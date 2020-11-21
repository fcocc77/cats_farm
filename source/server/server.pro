include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "vserver"

CONFIG += console
INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ./

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../manager/structs.h \
			../utils/video.h \
			../../utils/c++/*.h \

SOURCES += 	./*.cc \
			../utils/video.cc \
			../../utils/c++/* \
