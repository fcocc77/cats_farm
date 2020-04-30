# "silent" solo muestra el nombre del cpp y no todos los includes en la conpilacion
CONFIG += silent
# ----------------------------

# ignora todos los mensajes "warning"
QMAKE_CXXFLAGS += -w
CONFIG += warn_off
# ----------------------------

CONFIG += c++11
CONFIG -= app_bundle

DESTDIR = release
OBJECTS_DIR = release/.obj
MOC_DIR = release/.moc
RCC_DIR = release/.rcc
UI_DIR = release/.ui