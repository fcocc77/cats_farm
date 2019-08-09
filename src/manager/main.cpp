#include "manager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	manager _manager;
	_manager.init();

	return app.exec();
}
