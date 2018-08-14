# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
RC_ICONS = ../../icons/server.ico

TEMPLATE = app
TARGET = "CatsFarm Server"

CONFIG += console c++14

QT += core gui multimedia widgets

HEADERS += 	./server.h \
			./render.h \
			../manager/structs.h \
			../utils/json.h \
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
