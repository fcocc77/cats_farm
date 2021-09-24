include(../../config/config.pro)

TEMPLATE = app
TARGET = "vmonitor"

INCLUDEPATH += $$files(*)
INCLUDEPATH += ../utils
INCLUDEPATH += ../manager
INCLUDEPATH += ../../cpp_util
INCLUDEPATH += ../../global
INCLUDEPATH += ../../libs/QCodeEditor/include
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL/Design
INCLUDEPATH += ../../libs/QCodeEditor/include/KGL/Widgets

RC_ICONS = ../../resources/icons/icon.ico

QT += core gui widgets multimedia

HEADERS +=  $$system(ls */*.h) \
			../utils/video.h \
			../global/*.h \
			../../cpp_util/*.h \
			../manager/*.h \
			$$system(ls ../../libs/QCodeEditor/include/KGL/*/*.hpp) \

SOURCES += 	$$system(ls */*.cc) \
			../utils/video.cc \
            ../../cpp_util/*.cc \
            $$system(ls ../../libs/QCodeEditor/src/*/*.cpp) \

RESOURCES += \
	# QCodeEditor
    ../../libs/QCodeEditor/files/res.qrc
