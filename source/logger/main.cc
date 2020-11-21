#include <logger.hpp>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	logger _logger;

	return a.exec();
}
