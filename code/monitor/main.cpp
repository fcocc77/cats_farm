#include "main_window.h"

int main( int argc, char *argv[] ){
	string showMonitor = path() + "/etc/showMonitor";
	
	// si el monitor esta abierto no lo abre
	int count = 0;
	if  ( _win32 ){ 
		auto lista = split( os::sh( "tasklist -fi \"IMAGENAME eq monitor.exe\"" ), "\n" );
		for ( string l : lista ) if ( in_string( "monitor.exe", l ) ) count++;
	}
	else count = 1;
	//------------------------------------------

	if ( count == 1 ){
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

 