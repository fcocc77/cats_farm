#include "server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	server s;
	s.init();

	return a.exec();
}
