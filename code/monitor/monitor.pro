# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
TEMPLATE = app
TARGET = "CatsFarm Monitor"
CONFIG += c++11 console
INCLUDEPATH += ../external/QCodeEditor/include # esto es para QCodeEditor

RC_ICONS = ../../icons/monitor.ico

QT += core gui widgets multimedia

HEADERS += 	./general.h \
			./get_manager_info.h \
			./group_actions.h \
			./jobs_actions.h \
			./main_window.h \
			./servers_actions.h \
			./settings.h \
			./shared_variables.h \
			./tasks_actions.h \
			./ui_groups_tree.h \
			./ui_jobs_tree.h \
			./ui_job_options.h \
			./ui_servers_tree.h \
			./ui_server_options.h \
			./ui_submit.h \
			./ui_denoiser.h \
			./ui_tasks_tree.h \
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

SOURCES += 	./main.cpp \
			./main_window.cpp \
			./general.cpp \
			./get_manager_info.cpp \
			./groups_actions.cpp \
			./jobs_actions.cpp \
			./servers_actions.cpp \
			./settings.cpp \
			./tasks_actions.cpp \
			./ui_groups_tree.cpp \
			./ui_jobs_tree.cpp \
			./ui_job_options.cpp \
			./ui_servers_tree.cpp \
			./ui_server_options.cpp \
			./ui_submit.cpp \
			./ui_denoiser.cpp \
			./ui_tasks_tree.cpp \
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
