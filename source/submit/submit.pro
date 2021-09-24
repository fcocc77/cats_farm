include(../../config/config.pro)

TEMPLATE = app
TARGET = submit
RC_ICONS = ../../resources/icons/icon.ico

CONFIG += console
INCLUDEPATH += ./
INCLUDEPATH += ../../cpp_util

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../utils/tcp.hpp \
			../../cpp_util/*.h \

SOURCES += 	./*.cc \
			../../cpp_util/*.cc \
