#ifndef TPC_H
#define TPC_H

#include <iostream>
using namespace std;
#include "util.h"

// QT5 Library
#include <unistd.h>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
//----------------------

template < class T >
class tcp_socket : public QThread{
public:

	int port; 
	T *_class; 
	QJsonArray ( T::*func )( QJsonArray, int );

	QTcpSocket *qsocket;    
	int socketDescriptor;

	tcp_socket( int _socketDescriptor, int _port, QJsonArray ( T::*_func )( QJsonArray, int ), T *__class ){
		port = _port;
		func = _func;
		_class = __class;
		socketDescriptor = _socketDescriptor;
	}

	void run(){
		qsocket = new QTcpSocket();
		if ( !qsocket->setSocketDescriptor( socketDescriptor )) return;

		QString send;
		QString recv;
		QJsonArray _recv = {};
		QJsonArray _pks = {};
		int _input = 0;
		int wait = -1; // en -1 significa que no tiene timeout para los waitForBytesWritten y waitForReadyRead
		// waitForConnected: espera que un cliente se conecte a este socket
		while ( qsocket->waitForConnected(5000) ){

			// 2 recive tamanio de paquete
			int size;
			qsocket->waitForReadyRead(wait);
			size = qsocket->readAll().toInt();
			//------------------------------------

			//3
			qsocket->write("ok");
			qsocket->waitForBytesWritten(wait);
			//

			// 6 recive paquete
			recv = "";
			int totalBytesRead = 0;
			while (totalBytesRead < size ) {
				if ( qsocket->waitForReadyRead(wait) ) {
						totalBytesRead += qsocket->bytesAvailable();
						recv +=  qsocket->readAll();
				}       
			}
			//----------------------------------------------------

			/* Si se desconecta el cliente, no termina la transferencia de los
			paquete, y el json al hacer parse da un error, por eso try except.*/
			bool recv_ok = false;
			try{ 
				_recv = QJsonDocument::fromJson( recv.toUtf8() ).array();
				_pks = _recv[0].toArray();
				_input = _recv[1].toInt();
				recv_ok = true;
			}
			catch( exception& e ){}

			if ( recv_ok ){
				try{
					QJsonDocument _doc( ( _class->*func )( _pks, _input ) );
					send = _doc.toJson( QJsonDocument::Compact );

				}
				catch(...){
					send = "";
				}
			}
			else send = "";
			//-----------------------------------------------------------

			// 7 - envia tamanio de paquete
			// cuando el cliente de desconecta da un error, si no da el error, espera los byte escritos
			qsocket->write( QString::number( send.size()).toStdString().c_str() );
			qsocket->waitForBytesWritten(wait);
			//-------------------------------------------------

			//10 recive info de loop
			qsocket->waitForReadyRead(wait);
			QString loop = qsocket->readAll();
			//--------------------------------------------
			//print(send);
			// 11 envia paquete
			qsocket->write(send.toStdString().c_str());
			qsocket->waitForBytesWritten(wait);
			//-------------------------------------

			if ( loop == "false" ) {
				qsocket->waitForReadyRead(wait); // espera respuesta para terminar
				break;
			} 
		}

		qsocket->close();
	}
};

template < class T >
class tcp_server : public QTcpServer{
public:
	int port; 
	T *_class; 
	QJsonArray ( T::*func )( QJsonArray, int );
	QTcpServer *qserver;

	// Constructor para servidor ( siempre esta en loop y con qthread ) 
	tcp_server( int _port, QJsonArray ( T::*_func )( QJsonArray, int ), T *__class ) : QTcpServer(__class){        
		port = _port;
		func = _func;
		_class = __class;
	} //---------------------------------------------------------------

	void init(){
		if(!this->listen( QHostAddress::Any , port))
		{
			qDebug() << "Server could not start!";
		}
		else
		{
			qDebug() << "Server started!";
		}
	}

protected:
	void incomingConnection( qintptr socketDescriptor ){
		tcp_socket<T> *_tcp_socket = new tcp_socket<T>( socketDescriptor, port, func, _class );
		_tcp_socket->start();
	}
};

template < class T >
class tcp_client_widget : public QThread{
public:
	T *_class; 
	QJsonArray ( T::*func )( QJsonArray );
	QTimer *qtimer;
	bool widget;

	QJsonArray *update_send;
	QJsonArray *update_recv;
	bool *send_ready;
	bool *recv_ready;

	// Constructor client loop
	tcp_client_widget( QJsonArray ( T::*_func )( QJsonArray ), T *__class, QJsonArray *_update_send, QJsonArray *_update_recv, 
									bool *_send_ready, bool *_recv_ready, bool _widget ) : QThread(__class) {
		_class = __class;
		func = _func;
		update_send = _update_send;
		update_recv = _update_recv;
		send_ready = _send_ready;
		recv_ready = _recv_ready;
		widget = _widget;

		// hilo loop para widget, se inicia por fuera
		qtimer = new QTimer();
		connect ( qtimer, &QTimer::timeout, this, &tcp_client_widget::widget_update );
		qtimer->moveToThread(this);

		widget_update();
		//----------------------------------
	} //--------------------------

	void run(){
		// si es que el la funcion actualiza con los widget de QT, usa un QTimer para que no se pegue,
		// si no usa un hilo normal
		if ( widget ) {
			qtimer->start(1000);
			exec();
		}

		else {
			while(1){
				widget_update();
				sleep(1);
			}
		}
		//-----------------------------------------------------------------

	}

	void widget_update(){

		/* recv_ready y send_ready son para que json no de error de memoria
		cuando se intercambian las variable entre client y client_widget */
		*recv_ready = false;

		if ( *send_ready ){ 
			QJsonArray _send = *update_send;
			*update_recv = ( _class->*func )( _send );
		}

		*recv_ready = true; 
	}
};

class tcp_client : public QThread{
public:
	QString host;
	int port; 
	bool loop; 
	QJsonArray pks;
	int input;

	QJsonArray update_send;
	QJsonArray update_recv;
	bool send_ready; // para que json no de error cuando se intercambian las variable entre client y client_widget
	bool recv_ready = true;
	// Constructor client loop
	template < class T >
	tcp_client( QString _host, int _port,  T *_class, int _input ){
		host = _host;
		port = _port;
		input = _input;
		loop = true;
		update_send = {};
		update_recv = {};

	} //-----------------------------

	// Costructor para packete unico
	tcp_client( QString _host, int _port, QJsonArray _pks, int _input ){
		host = _host;
		port = _port;
		input = _input;
		pks = { _pks , _input };
		loop = false;
	} //-----------------------------

	void run(){
		client();
	}

	QJsonArray client(){

		QTcpSocket *socket = new QTcpSocket();
		int wait = -1;
		QJsonDocument doc(pks);
		QString send = doc.toJson( QJsonDocument::Compact );
		QString recv;
		QJsonArray _recv = {};

		while (1){ 
			socket->connectToHost( host, port );

			while ( socket->state() ){ // si el server se deconecta se rompe el loop

				if ( loop ) {
					/* recv_ready y send_ready son para que json no de error de memoria
					cuando se intercambian las variable entre client y client_widget */
					send_ready = false;

					QJsonArray _send;
					if ( recv_ready ) {
						update_send = _recv;
						_send = { update_recv, input };

					}
					else {
						/* Cuando el recv_ready no esta listo
						el loop continua en la siguiente iteracion
						por que si sigue da conflicto en el server
						al recibir los paquetes y se queda pegado */
						sleep(1);
						continue;
					}

					send_ready = true;

					try{ 
						QJsonDocument _doc(_send);
						send = _doc.toJson( QJsonDocument::Compact );
					}
					catch( exception& e ){ send = ""; }

				}

				// 1 - envia tamanio de paquete
				socket->write( to_string(send.size()).c_str() );
				socket->waitForBytesWritten(wait); 
				//-------------------------------------------------

				//4
				socket->waitForReadyRead(wait);
				socket->readAll();

				// 5 envia paquete
				socket->write(send.toStdString().c_str());
				socket->waitForBytesWritten(wait);
				//-------------------------------------

				// 8 recive tamanio de paquete
				int size;
				socket->waitForReadyRead(wait);
				size = socket->readAll().toInt();
				//------------------------------------

				//9 envia si la accion si es en loop o no
				if ( loop ){ socket->write("true"); }
				else{ socket->write("false"); }
				socket->waitForBytesWritten(wait); 
				//-------------------------------------------------

				// 12 recive paquete
				recv = "";
				int totalBytesRead = 0;
				while (totalBytesRead < size ) {
					if ( socket->waitForReadyRead(wait) ) {							
							totalBytesRead += socket->bytesAvailable();
							recv +=  socket->readAll();
					}       
				}
				//----------------------------------------------------

				/* Si se desconecta el servidor, no termina la transferencia de los
				paquete, y el json al hacer parse da un error, por eso try except.*/
				try{ 
					_recv = QJsonDocument::fromJson( recv.toUtf8() ).array();
				}
				catch( exception& e ){}
				//-----------------------------------------------------------

				if ( loop ){ sleep(1); }
				else{ break; }

			}
			socket->close();

			if ( loop ) sleep(3);
			else return _recv;
		}
		return {};
	}
};

template < class T >
void tcpServer(  int _port, QJsonArray ( T::*_func )( QJsonArray, int ), T *_class ){
	tcp_server< T > *_server = new tcp_server< T >( _port, _func, _class ); 
	_server->init();
}

template < class T >
void tcpClient(  QString _host, int _port, QJsonArray ( T::*_func )( QJsonArray ), T *_class, int _input, bool widget = false ){
	// inicia thread de tcp socket
	tcp_client *_client = new tcp_client( _host, _port, _class, _input ); 
	_client->exit();
	_client->start();
	//-------------------------------------

	// inicia thread de update widget
	tcp_client_widget < T >  *_widget = new tcp_client_widget < T >( _func, _class, &_client->update_send, 
								&_client->update_recv,  &_client->send_ready, &_client->recv_ready, widget );     
	_widget->exit();
	_widget->start();
	//-------------------------------------
}

template < class T >
QJsonArray tcpClient( T _host, int _port, QJsonArray _pks, int _input ){
	tcp_client *_client = new tcp_client( _host, _port, _pks, _input ); 
	_client->exit();
	return _client->client();
}

#endif //TPC_H