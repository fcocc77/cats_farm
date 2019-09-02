#include "../utils/util.h"
#include "../utils/os.h"
#include "../utils/tcp.h"
#include "../utils/video.h"
#include "../utils/threading.h"

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

    // Mata las pid a partir del nombre "d" (es el after effect render)
    QStringList ps = os::sh("ps cax").split("\n");
    for (QString line : ps)
    {
        QString lastWord = line.split(" ").back();
        if (lastWord == "d")
        {
            int pid = line.split(" ")[0].toInt();
            os::kill(pid);
        }
    }
    // -------------------------------------------



}
