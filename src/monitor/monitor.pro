# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
TEMPLATE = app
TARGET = "cmonitor"
CONFIG += c++11
INCLUDEPATH += ../external/QCodeEditor/include # esto es para QCodeEditor
CONFIG -= app_bundle
RC_ICONS = ../../icons/monitor.ico

QT += core gui widgets multimedia

HEADERS += 	./hpp/general.h \
			./hpp/get_manager_info.h \
			./hpp/group_actions.h \
			./hpp/jobs_actions.h \
			./hpp/main_window.h \
			./hpp/servers_actions.h \
			./hpp/settings.h \
			./hpp/shared_variables.h \
			./hpp/tasks_actions.h \
			./hpp/ui_groups_tree.h \
			./hpp/ui_jobs_tree.h \
			./hpp/ui_job_options.h \
			./hpp/ui_servers_tree.h \
			./hpp/ui_server_options.h \
			./hpp/ui_submit.h \
			./hpp/ui_tasks_tree.h \
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
			./cpp/main_window.cpp \
			./cpp/general.cpp \
			./cpp/get_manager_info.cpp \
			./cpp/groups_actions.cpp \
			./cpp/jobs_actions.cpp \
			./cpp/servers_actions.cpp \
			./cpp/settings.cpp \
			./cpp/tasks_actions.cpp \
			./cpp/ui_groups_tree.cpp \
			./cpp/ui_jobs_tree.cpp \
			./cpp/ui_job_options.cpp \
			./cpp/ui_servers_tree.cpp \
			./cpp/ui_server_options.cpp \
			./cpp/ui_submit.cpp \
			./cpp/ui_tasks_tree.cpp \
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
