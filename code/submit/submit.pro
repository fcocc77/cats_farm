# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = submit
RC_ICONS = ../../icons/monitor.ico

CONFIG += console c++14

QT += core gui multimedia widgets

HEADERS += 	./submit.h \
			../utils/tcp.h \
			../utils/util.h \
			../utils/os.h \
			../utils/json.h \

SOURCES += 	./main.cpp \
			./submit.cpp \
			../utils/util.cpp \
			../utils/os.cpp \
