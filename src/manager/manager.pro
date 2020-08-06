include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmanager"

RC_ICONS = ../../icons/icon.ico

CONFIG += console
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
			./jobs.cpp \
			./servers.cpp \
			./groups.cpp \
			./tasks.cpp \
			./render.cpp \
			./update.cpp \
			./videovina.cpp \
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \