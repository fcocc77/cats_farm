include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "logger"

CONFIG += console
INCLUDEPATH += ./includes
INCLUDEPATH += ../../utils/c++/includes

QT += core gui multimedia widgets

HEADERS += 	./includes/* \
			../utils/tcp.hpp \
			../utils/threading.hpp \
			../utils/video.hpp \
			../../utils/c++/includes/* \

SOURCES += 	./src/* \
			../utils/video.cpp \
			../../utils/c++/src/* \
