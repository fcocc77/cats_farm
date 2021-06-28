#ifndef GLOBAL_H
#define GLOBAL_H

#include <os.h>

#define VINARENDER_SOURCE QString("/home/pancho/Documents/develop/vinarender")

#if _win32

#define USER_DIR QString::fromStdString(getenv("USERPROFILE"))
#define VINARENDER_CONF_PATH USER_DIR + "/AppData/Local/vinarender"
#define VINARENDER_PATH QString("C:/Program Files/vinarender")

#elif _linux

#define USER_DIR os::get_home()
#define VINARENDER_CONF_PATH USER_DIR + "/.vinarender"
#define VINARENDER_PATH QString("/opt/vinarender")

#endif

#endif // GLOBAL_H
