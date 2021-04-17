#ifndef TCP_HPP
#define TCP_HPP

#include <iostream>
using namespace std;
#include "util.h"

// QT5 Library
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>
#include <unistd.h>
//----------------------

template <class T> class tcp_socket : public QThread
{
public:
    int port;
    T *_class;
    QString (T::*func)(QString);

    QTcpSocket *qsocket;
    int socketDescriptor;

    tcp_socket(int _socketDescriptor, int _port, QString (T::*_func)(QString),
               T *__class)
    {
        port = _port;
        func = _func;
        _class = __class;
        socketDescriptor = _socketDescriptor;
    }

    void run()
    {
        qsocket = new QTcpSocket();
        if (!qsocket->setSocketDescriptor(socketDescriptor))
            return;

        QString send;
        QString recv;
        int wait = -1; // en -1 significa que no tiene timeout para los
                       // waitForBytesWritten y waitForReadyRead
        // waitForConnected: espera que un cliente se conecte a este socket
        while (qsocket->waitForConnected(5000))
        {

            // 2 recive tamanio de paquete
            int size;
            qsocket->waitForReadyRead(wait);
            size = qsocket->readAll().toInt();
            //------------------------------------

            // 3
            qsocket->write("ok");
            if (not qsocket->error())
                qsocket->waitForBytesWritten(wait);
            //

            // 6 recive paquete
            recv = "";
            int totalBytesRead = 0;
            while (totalBytesRead < size)
            {
                if (qsocket->waitForReadyRead(wait))
                {
                    totalBytesRead += qsocket->bytesAvailable();
                    recv += qsocket->readAll();
                }
                else
                    break;
            }
            //----------------------------------------------------

            send = (_class->*func)(recv);

            // 7 - envia tamanio de paquete
            // cuando el cliente de desconecta da un error, si no da el error,
            // espera los byte escritos
            qsocket->write(QString::number(send.size()).toStdString().c_str());
            if (not qsocket->error())
                qsocket->waitForBytesWritten(wait);
            //-------------------------------------------------

            // 10 recive info de loop
            qsocket->waitForReadyRead(wait);
            QString loop = qsocket->readAll();
            //--------------------------------------------
            // print(send);
            // 11 envia paquete
            qsocket->write(send.toStdString().c_str());
            if (not qsocket->error())
                qsocket->waitForBytesWritten(wait);
            //-------------------------------------

            if (loop == "false")
            {
                qsocket->waitForReadyRead(
                    wait); // espera respuesta para terminar
                break;
            }
        }

        qsocket->close();
    }
};

template <class T> class tcp_server : public QTcpServer
{
public:
    int port;
    T *_class;
    QString (T::*func)(QString);
    QTcpServer *qserver;

    // Constructor para servidor ( siempre esta en loop y con qthread )
    tcp_server(int _port, QString (T::*_func)(QString), T *__class)
        : QTcpServer(__class)
    {
        port = _port;
        func = _func;
        _class = __class;
    } //---------------------------------------------------------------

    void init()
    {
        if (!this->listen(QHostAddress::Any, port))
        {
            qDebug() << "Server could not start!";
        }
        else
        {
            qDebug() << "Server started!";
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor)
    {
        tcp_socket<T> *_tcp_socket =
            new tcp_socket<T>(socketDescriptor, port, func, _class);
        _tcp_socket->start();
    }
};

class tcp_client : public QThread
{
public:
    QString host;
    int port;
    bool loop;
    QString pks;

    QString update_send;
    QString update_recv;

    QMutex mutex;

    // Constructor client loop
    template <class T> tcp_client(QString _host, int _port, T *_class)
    {
        host = _host;
        port = _port;
        loop = true;

    } //-----------------------------

    // Costructor para packete unico
    tcp_client(QString _host, int _port, QString _pks)
    {
        host = _host;
        port = _port;
        pks = _pks;
        loop = false;
    } //-----------------------------

    void run()
    {
        client();
    }

    QString client()
    {

        QTcpSocket *socket = new QTcpSocket();
        int wait = -1;
        QString send = pks;
        QString recv;
        QString _recv;

        while (1)
        {
            socket->connectToHost(host, port);

            while (socket->state())
            { // si el server se deconecta se rompe el loop

                if (loop)
                {
                    /* mutex es para que en las variables compartidas no den
                    error de memoria,
                    esto pasa cuando se intercambian los datos entre client y
                    client_widget */
                    mutex.lock();
                    update_send = _recv;
                    send = update_recv;
                    mutex.unlock();
                    // ---------------------------
                }

                // 1 - envia tamanio de paquete
                socket->write(to_string(send.size()).c_str());
                if (not socket->error())
                    socket->waitForBytesWritten(wait);
                //-------------------------------------------------

                // 4
                socket->waitForReadyRead(wait);
                socket->readAll();

                // 5 envia paquete
                socket->write(send.toStdString().c_str());
                if (not socket->error())
                    socket->waitForBytesWritten(wait);
                //-------------------------------------

                // 8 recive tamanio de paquete
                int size;
                socket->waitForReadyRead(wait);
                size = socket->readAll().toInt();
                //------------------------------------

                // 9 envia si la accion si es en loop o no
                if (loop)
                    socket->write("true");
                else
                    socket->write("false");
                if (not socket->error())
                    socket->waitForBytesWritten(wait);
                //-------------------------------------------------

                // 12 recive paquete
                recv = "";
                int totalBytesRead = 0;
                while (totalBytesRead < size)
                {
                    if (socket->waitForReadyRead(wait))
                    {
                        totalBytesRead += socket->bytesAvailable();
                        recv += socket->readAll();
                    }
                    else
                        break;
                }
                //----------------------------------------------------

                _recv = recv;

                if (loop)
                    sleep(1);
                else
                    break;
            }
            socket->close();

            if (loop)
                sleep(3);
            else
                return _recv;
        }
        return {};
    }
};

template <class T> class tcp_client_widget : public QThread
{
public:
    T *_class;
    QString (T::*func)(QString, QJsonObject);
    QTimer *qtimer;
    bool widget;
    QJsonObject extra;

    tcp_client *client;

    // Constructor client loop
    tcp_client_widget(QString (T::*_func)(QString, QJsonObject), T *__class,
                      tcp_client *_client, bool _widget, QJsonObject _extra)
        : QThread(__class)
    {
        _class = __class;
        func = _func;
        client = _client;
        widget = _widget;
        extra = _extra;

        // hilo loop para widget, se inicia por fuera
        qtimer = new QTimer();
        connect(qtimer, &QTimer::timeout, this,
                &tcp_client_widget::widget_update);
        qtimer->moveToThread(this);

        widget_update();
        //----------------------------------
    } //--------------------------

    void run()
    {
        // si es que el la funcion actualiza con los widget de QT, usa un QTimer
        // para que no se pegue, si no usa un hilo normal
        if (widget)
        {
            qtimer->start(1000);
            exec();
        }

        else
        {
            while (1)
            {
                widget_update();
                sleep(1);
            }
        }
        //-----------------------------------------------------------------
    }

    void widget_update()
    {
        /* mutex es para que en las variables compartidas no den error de
        memoria, esto pasa cuando se intercambian los datos entre client y
        client_widget */
        client->mutex.lock();
        QString _send = client->update_send;
        client->update_recv = (_class->*func)(_send, extra);
        client->mutex.unlock();
        // ---------------------------
    }

    void kill()
    {
        // termina los 2 hilos de client
        client->terminate();
        this->terminate();
        // -----------------------
    }
};

template <class T>
void tcpServer(int _port, QString (T::*_func)(QString), T *_class)
{
    tcp_server<T> *_server = new tcp_server<T>(_port, _func, _class);
    _server->init();
}

template <class T>
tcp_client_widget<T> *
tcpClient(QString _host, int _port, QString (T::*_func)(QString, QJsonObject),
          T *_class, bool widget = false, QJsonObject extra = {})
{
    // inicia thread de tcp socket
    tcp_client *_client = new tcp_client(_host, _port, _class);
    _client->exit();
    _client->start();
    //-------------------------------------

    // inicia thread de update widget
    tcp_client_widget<T> *_widget =
        new tcp_client_widget<T>(_func, _class, _client, widget, extra);
    _widget->exit();
    _widget->start();
    //-------------------------------------

    return _widget;
}

template <class T> QString tcpClient(T _host, int _port, QString _pks)
{
    tcp_client *_client = new tcp_client(_host, _port, _pks);
    _client->exit();
    return _client->client();
}

#endif // TCP_HPP
