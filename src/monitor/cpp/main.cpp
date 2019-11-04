#include "../hpp/main_window.hpp"

int main(int argc, char *argv[])
{
	QString showMonitor = path + "/etc/showMonitor";

	// si el monitor esta abierto no lo abre
	int count = 0;
	if (_win32)
	{
		QStringList lista = os::sh("tasklist -fi \"IMAGENAME eq vmonitor.exe\"").split("\n");
		for (QString l : lista)
			if (l.contains("vmonitor.exe"))
				count++;
	}
	else
		count = 1;
	//------------------------------------------

	if (count == 1)
	{
		QApplication a(argc, argv);
		monitor w;
		// w.init();
		w.showMaximized();

		return a.exec();
	}

	else
	{
		fwrite(showMonitor, "1");
		return 0;
	}
}
