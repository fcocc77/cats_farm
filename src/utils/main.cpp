#include <util.hpp>
#include <os.hpp>
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

   os::sh("sh -c \"sync && sysctl -w vm.drop_caches=3\"");
}
