include(../../config/config.pro)

TEMPLATE = app
TARGET = submit
RC_ICONS = ../../resources/icons/icon.ico

CONFIG += console
INCLUDEPATH += ./
INCLUDEPATH += ../../utils/c++

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../utils/tcp.hpp \
			../../utils/c++/*.h \

SOURCES += 	./*.cc \
			../../utils/c++/*.cc \
