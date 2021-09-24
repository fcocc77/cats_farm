include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "logger"

CONFIG += console
INCLUDEPATH += ./
INCLUDEPATH += ../utils
INCLUDEPATH += ../../cpp_util

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../utils/*.h \
			../../cpp_util/*.h \

SOURCES += 	./*.cc \
			../utils/*.cc \
			../../cpp_util/*.cc \
