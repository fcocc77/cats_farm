TEMPLATE = app
TARGET = denoiser
CONFIG += console c++14

QT += core gui multimedia widgets

HEADERS += 	denoiser.h \
			../../code/utils/util.h \
			../../code/utils/os.h \
			../../code/utils/tcp.h \
			../../code/utils/json.h 

SOURCES += 	main.cpp \
			denoiser.cpp \
			../../code/utils/os.cpp \
			../../code/utils/util.cpp 
