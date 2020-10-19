include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmonitor"

CODE_EDITOR = ../external/QCodeEditor/include
MONITOR_INCLUDES = ./includes

INCLUDEPATH += ../../utils/c++/includes
INCLUDEPATH += $$MONITOR_INCLUDES
INCLUDEPATH += $$CODE_EDITOR # esto es para QCodeEditor

RC_ICONS = ../../icons/icon.ico

QT += core gui svg widgets multimedia

HEADERS +=  $$MONITOR_INCLUDES/* \
			../utils/*.hpp \
			../../utils/c++/includes/* \
			../manager/includes/structs.hpp \
			$$CODE_EDITOR/KGL/Design/* \
			$$CODE_EDITOR/KGL/Widgets/* \
			$$CODE_EDITOR/KGL/KGLConfig.hpp \

SOURCES += 	./src/* \
			../utils/video.cpp \
			../../utils/c++/src/* \
			../external/QCodeEditor/src/Design/*.cpp \
			../external/QCodeEditor/src/Widgets/*.cpp \

RESOURCES += \
	# QCodeEditor
    ../external/QCodeEditor/files/res.qrc