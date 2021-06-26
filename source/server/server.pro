include(../../config/config.pro)

RC_ICONS = ../../resources/icons/icon.ico

TEMPLATE = app
TARGET = "vserver"

CONFIG += console
INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ../utils
INCLUDEPATH += ./

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../manager/structs.h \
			../utils/video.h \
			../utils/hardware_monitor.h \
			../../utils/c++/*.h \

SOURCES += 	./*.cc \
			../utils/video.cc \
			../utils/hardware_monitor.cc \
			../../utils/c++/*cc \
