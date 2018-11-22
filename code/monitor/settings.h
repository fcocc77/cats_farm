#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
using namespace std;
#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/threading.h"

//inclide QT5
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QString>
#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QObject>
#include <QStyle>
#include <QDesktopWidget>
#include <QDebug>
//-------------

class settings : public QMainWindow{
Q_OBJECT
public:
 	QMainWindow *monitor;

 	// este es el constructor
	template < class T >
	settings(  T *_monitor  ) : QMainWindow( _monitor){  //){  , 
		monitor = _monitor;
		ui();
		style();

		pathSusRead();
	}; 
	//---------------------------

private:

    //QWidgets Generales
	const QString managerHost = fread( "../../etc/manager_host" );
	QLineEdit *manager_ip = new QLineEdit( managerHost );
	QTabWidget *tab_widget = new QTabWidget();
	QPlainTextEdit *system_edit = new QPlainTextEdit();
	QPlainTextEdit *maya_edit = new QPlainTextEdit();
	QPlainTextEdit *nuke_edit = new QPlainTextEdit();
	QPlainTextEdit *houdini_edit = new QPlainTextEdit();
	QPlainTextEdit *cinema_edit = new QPlainTextEdit();
	QPlainTextEdit *fusion_edit = new QPlainTextEdit();
	QPlainTextEdit *natron_edit = new QPlainTextEdit();

    //------------------------------

    // declaracion de funciones

    void ui();
    void style();
    QWidget *general_ui();
    void path_ui();
    void pathSusRead();
    void pathSusWrite();
    //--------------------------

};

#endif //SETTINGS_H