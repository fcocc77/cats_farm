#include "../utils/util.hpp"
#include "../utils/os.hpp"
#include "../utils/tcp.hpp"
#include "../utils/video.hpp"
#include "../utils/threading.hpp"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>
#include <QList>
#include <unistd.h> // sleep usleep

int main()
{

    print(os::mac());
}
