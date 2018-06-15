#include "main_window.h"


int main( int argc, char *argv[] ){
	QApplication a(argc, argv);
	monitor w;

	w.init();
	w.showMaximized();


    return a.exec(); 
}


 