include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "logger"

CONFIG += console

QT += core gui multimedia widgets

HEADERS += 	./logger.hpp \
			../utils/os.hpp \
			../utils/tcp.hpp \
			../utils/threading.hpp \
			../utils/util.hpp \
			../utils/video.hpp \

SOURCES += 	./main.cpp \
			./logger.cpp \  
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \