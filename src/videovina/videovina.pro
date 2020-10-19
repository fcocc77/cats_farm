include(../../config/config.pro)

TEMPLATE = app
TARGET = videovina
RC_ICONS = ../../icons/server.ico

CONFIG += console

QT += core gui multimedia widgets

INCLUDEPATH += ./includes
INCLUDEPATH += ../../utils/c++/includes

QT += core gui multimedia widgets

HEADERS += 	./includes/* \
			../utils/tcp.hpp \
			../../utils/c++/includes/* \

SOURCES += 	./src/* \
			../../utils/c++/src/* \
