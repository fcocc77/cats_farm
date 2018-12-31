# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
RC_ICONS = ../../icons/server.ico

TEMPLATE = app
TARGET = "cServer"

CONFIG += console c++11 
CONFIG -= app_bundle

QT += core gui multimedia widgets

HEADERS += 	./server.h \
			./render.h \
			../manager/structs.h \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \
			../utils/util.h \
			../utils/video.h \

SOURCES += 	./main.cpp \
			./server.cpp \  
			./render.cpp \ 
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \
