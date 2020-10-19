include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "vserver"

CONFIG += console
INCLUDEPATH += ../../utils/c++/includes
INCLUDEPATH += ./includes

QT += core gui multimedia widgets

HEADERS += 	./includes/* \
			../manager/structs.hpp \
			../utils/tcp.hpp \
			../utils/threading.hpp \
			../utils/video.hpp \
			../../utils/c++/includes/* \

SOURCES += 	./src/* \
			../utils/video.cpp \
			../../utils/c++/src/* \
