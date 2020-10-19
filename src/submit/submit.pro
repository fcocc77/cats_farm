include(../../config/config.pro)

TEMPLATE = app
TARGET = submit
RC_ICONS = ../../icons/icon.ico

CONFIG += console
INCLUDEPATH += ./includes
INCLUDEPATH += ../../utils/c++/includes

QT += core gui multimedia widgets

HEADERS += 	./includes/* \
			../utils/tcp.hpp \
			../../utils/c++/includes/* \

SOURCES += 	./src/* \
			../../utils/c++/src/* \
