# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = submit
RC_ICONS = ../../icons/icon.ico

CONFIG += console c++11

QT += core gui multimedia widgets

HEADERS += 	./submit.hpp \
			../utils/tcp.hpp \
			../utils/util.hpp \
			../utils/os.hpp \

SOURCES += 	./main.cpp \
			./submit.cpp \
			../utils/util.cpp \
			../utils/os.cpp \
