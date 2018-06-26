
# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
RC_ICONS = ../../icons/server.ico

TEMPLATE = app
TARGET = ./server

CONFIG += console c++14 debug

QT += core gui multimedia widgets

HEADERS += 	./server.h \
			../manager/structs.h \
			../utils/json.h \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \
			../utils/util.h \
			../utils/video.h \

SOURCES += 	./main.cpp \
			./server.cpp \  
			./renderRun.cpp \ 
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \
