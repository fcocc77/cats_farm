# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = "vmanager"

# "silent" solo muestra el nombre del cpp y no todos los includes en la conpilacion
CONFIG += silent

RC_ICONS = ../../icons/icon.ico

CONFIG += console c++11
CONFIG -= app_bundle
QT += core gui multimedia widgets

HEADERS +=  ../utils/os.hpp \
			../utils/tcp.hpp \
			../utils/threading.hpp \ 
			../utils/util.hpp \
			../utils/video.hpp \
			./manager.hpp \
			./structs.hpp \

SOURCES += 	./main.cpp \
			./manager.cpp \  
			./actions.cpp \
			./render.cpp \
			./update.cpp \
			./videovina.cpp \
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \