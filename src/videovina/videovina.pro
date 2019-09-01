# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = videovina
RC_ICONS = ../../icons/server.ico

CONFIG += console c++11

QT += core gui multimedia widgets

HEADERS += 	./submit.h \
			../utils/tcp.h \
			../utils/util.h \
			../utils/os.h \

SOURCES += 	./main.cpp \
			./submit.cpp \
			../utils/util.cpp \
			../utils/os.cpp \