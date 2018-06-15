# disable unsused variables warning
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#---------------------------------
TEMPLATE = app
TARGET = ./monitor
CONFIG += c++14 console
CONFIG += debug 


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
			../utils/json.h \
			../utils/os.h \
			../utils/tcp.h \
			../utils/threading.h \
			../utils/util.h \
			../utils/video.h \
			../manager/structs.h

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

