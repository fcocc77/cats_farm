include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmonitor"

INCLUDEPATH += ./
INCLUDEPATH += ../utils
INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ../../global
INCLUDEPATH += ../../libs/QCodeEditor/include
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL/Design
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL/Widgets

RC_ICONS = ../../resources/icons/icon.ico

QT += core gui widgets multimedia

HEADERS +=  ./* \
			../utils/video.h \
			../global/*.h \
			../../utils/c++/*.h \
			../manager/structs.h \
			$$system(ls ../../libs/QCodeEditor/include/KGL/*/*.hpp) \

SOURCES += 	./*.cc \
			../utils/video.cc \
			../../utils/c++/*.cc \
			$$system(ls ../../libs/QCodeEditor/src/*/*.cpp) \

RESOURCES += \
	# QCodeEditor
    ../../libs/QCodeEditor/files/res.qrc
