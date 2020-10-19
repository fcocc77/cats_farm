include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmanager"

RC_ICONS = ../../icons/icon.ico

CONFIG += console
QT += core gui multimedia widgets

INCLUDEPATH += ../../utils/c++/includes
INCLUDEPATH += ./includes

HEADERS +=  ../utils/tcp.hpp \
			../utils/threading.hpp \ 
			../utils/video.hpp \
			./includes/* \
			../../utils/c++/includes/* \

SOURCES += 	./src/* \
			../utils/video.cpp \
			../../utils/c++/src/* \
