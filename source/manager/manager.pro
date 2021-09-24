include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmanager"

RC_ICONS = ../../resources/icons/icon.ico

CONFIG += console
QT += core gui multimedia widgets

INCLUDEPATH += ../../cpp_util
INCLUDEPATH += ../utils
INCLUDEPATH += ./

HEADERS +=  ./*.h \
			../utils/*.h \
			../../cpp_util/*.h \

SOURCES += 	./*.cc \
			../utils/*.cc \
			../../cpp_util/*.cc \
