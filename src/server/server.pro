include(../../config/config.pro)

RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "vserver"

CONFIG += console

QT += core gui multimedia widgets

HEADERS += 	./server.hpp \
			./render.hpp \
			../manager/structs.hpp \
			../utils/os.hpp \
			../utils/tcp.hpp \
			../utils/threading.hpp \
			../utils/util.hpp \
			../utils/video.hpp \

SOURCES += 	./main.cpp \
			./server.cpp \  
			./render.cpp \ 
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \
