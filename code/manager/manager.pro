# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------

TEMPLATE = app
TARGET = "CatsFarm Manager"
RC_ICONS = ../../icons/manager.ico

CONFIG += console c++14

QT += core gui multimedia widgets

HEADERS += 	../utils/json.h \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \ 
			../utils/util.h \
			../utils/video.h \
			./manager.h \
			./structs.h \

SOURCES += 	./main.cpp \
			./manager.cpp \  
			./actions.cpp \
			./render.cpp \
			./update.cpp \
			../utils/util.cpp \ 
			../utils/video.cpp \
			../utils/os.cpp \