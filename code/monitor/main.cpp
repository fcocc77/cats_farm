#include "main_window.h"

int main( int argc, char *argv[] ){
	string openMonitor = path() + "/etc/openMonitor";
	string showMonitor = path() + "/etc/showMonitor";
	// si el monitor esta abierto no lo abre
	if ( fread( openMonitor ) != "1" ){
		fwrite( openMonitor, "1" );

		QApplication a(argc, argv);
		monitor w;
		w.init();
		w.showMaximized();

	    return a.exec(); 
    }

    else {
    	fwrite( showMonitor, "1" );
		return 0;
    }
}

 