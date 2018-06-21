#include "server.h"

int main( int argc, char *argv[] ){
	QApplication a(argc, argv);

	server _server( argv[0] );

    return a.exec();
}

