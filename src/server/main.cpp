#include "server.hpp"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	server _server;

	return a.exec();
}
