#ifndef GLOBAL_H
#define GLOBAL_H

#define USER_DIR QString::fromStdString(getenv("HOME"))

#define VINARENDER_CONF_PATH USER_DIR + "/.vinarender"

#define VINARENDER_PATH QString("/opt/vinarender")

#endif // GLOBAL_H
