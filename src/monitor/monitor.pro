# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
TEMPLATE = app
TARGET = "vmonitor"
CONFIG += c++11
INCLUDEPATH += ../external/QCodeEditor/include # esto es para QCodeEditor
CONFIG -= app_bundle
RC_ICONS = ../../icons/monitor.ico

QT += core gui widgets multimedia

FORMS = ./ui/main.ui

HEADERS += 	./hpp/includes.hpp \
			./hpp/main_menu.hpp \
			./hpp/jobs.hpp \
			./hpp/servers.hpp \
			./hpp/tasks.hpp \
			./hpp/groups.hpp \
			./hpp/update.hpp \
			./hpp/main_window.hpp \
			./hpp/shared_variables.hpp \
			./hpp/global.hpp \
			./hpp/settings.hpp \
			./hpp/toolbar.hpp \
			./hpp/log.hpp \
			./hpp/options.hpp \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \
			../utils/util.h \
			../utils/video.h \
			../manager/structs.h \
			# QCodeEditor 
			../external/QCodeEditor/include/KGL/Design/QCodeEditorDesign.hpp \
			../external/QCodeEditor/include/KGL/Design/QCodeEditorSheets.hpp \
			../external/QCodeEditor/include/KGL/Design/QLineColumnPadding.hpp \
			../external/QCodeEditor/include/KGL/Design/QSyntaxRule.hpp \
			../external/QCodeEditor/include/KGL/Widgets/QCodeEditor.hpp \
			../external/QCodeEditor/include/KGL/Widgets/QCodeEditorHighlighter.hpp \
			../external/QCodeEditor/include/KGL/Widgets/QCodeEditorLineWidget.hpp \
			../external/QCodeEditor/include/KGL/Widgets/QCodeEditorPopup.hpp \
			../external/QCodeEditor/include/KGL/KGLConfig.hpp \
			../external/QCodeEditor/include/KGL/Design/XmlHelper.hpp \
			#-------------------------------------

SOURCES += 	./cpp/main.cpp \
			./cpp/main_menu.cpp \
			./cpp/jobs.cpp \
			./cpp/servers.cpp \
			./cpp/tasks.cpp \
			./cpp/groups.cpp \
			./cpp/main_window.cpp \
			./cpp/update.cpp \
			./cpp/global.cpp \
			./cpp/settings.cpp \
			./cpp/toolbar.cpp \
			./cpp/log.cpp \
			./cpp/options.cpp \
			../utils/os.cpp \
			../utils/util.cpp \
			../utils/video.cpp \
			# QCodeEditor
			../external/QCodeEditor/src/Design/QCodeEditorDesign.cpp \
			../external/QCodeEditor/src/Design/QCodeEditorSheets.cpp \
			../external/QCodeEditor/src/Design/QLineColumnPadding.cpp \
			../external/QCodeEditor/src/Design/QSyntaxRule.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditor.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditorEvents.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditorHighlighter.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditorLineWidget.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditorPopup.cpp \
			../external/QCodeEditor/src/Widgets/QCodeEditorSlots.cpp \
			../external/QCodeEditor/src/Design/XmlHelper.cpp \
			#-------------------------------------

RESOURCES += \
	# QCodeEditor
    ../external/QCodeEditor/files/res.qrc
    #---------------------------------------
