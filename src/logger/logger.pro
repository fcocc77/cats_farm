# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
RC_ICONS = ../../icons/manager.ico

TEMPLATE = app
TARGET = "logger"

CONFIG += console c++11 
CONFIG -= app_bundle

QT += core gui multimedia widgets

HEADERS += 	./logger.h \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \
			../utils/util.h \
			../utils/video.h \

SOURCES += 	./main.cpp \
			./logger.cpp \  
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \