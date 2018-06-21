#include "../utils/os.h"
#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QDebug>
#include <QString>
#include <QObject>

int main(){

	json result =  tcpClient("192.168.10.45", 7001, {}, 10 );

}