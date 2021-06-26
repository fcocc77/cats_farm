include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmanager"

RC_ICONS = ../../resources/icons/icon.ico

CONFIG += console
QT += core gui multimedia widgets

INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ../utils
INCLUDEPATH += ./

HEADERS +=  ./*.h \
			../utils/*.h \
			../../utils/c++/*.h \

SOURCES += 	./*.cc \
			../utils/*.cc \
			../../utils/c++/*.cc \
