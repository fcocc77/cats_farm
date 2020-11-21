include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "logger"

CONFIG += console
INCLUDEPATH += ./
INCLUDEPATH += ../../utils/c++

QT += core gui multimedia widgets

HEADERS += 	./*.h \
			../utils/*.h \
			../../utils/c++/*.h \

SOURCES += 	./*.h \
			../utils/*.cc \
			../../utils/c++/*.cc \
