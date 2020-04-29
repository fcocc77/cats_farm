# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
RC_ICONS = ../../icons/icon.ico

TEMPLATE = app
TARGET = "vserver"

CONFIG += console c++11 
CONFIG -= app_bundle

# "silent" solo muestra el nombre del cpp y no todos los includes en la conpilacion
CONFIG += silent

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
