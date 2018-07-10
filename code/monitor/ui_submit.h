#ifndef UI_SUBMIT_H
#define UI_SUBMIT_H

// QT5 Library
#include <QDockWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QDebug>
//-----------

// Utils Library
#include "../utils/os.h"
#include "../utils/tcp.h"
#include "../utils/json.h"
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"

using namespace nlohmann;
//----------

class combobox : public QComboBox{
Q_OBJECT
private slots:
	void mousePressEvent(QMouseEvent* event){
		emit clicked();
	    QComboBox::mousePressEvent(event);
    }

signals:
    void clicked();

};

class ui_submit : public QDockWidget{
Q_OBJECT
public:
    QMainWindow *monitor;
    ui_servers_tree *serverList;
    ui_groups_tree *groupList;
    bool savePanel = false;

    template < class T >
    ui_submit( T *_monitor ) : QDockWidget( _monitor ) {
        monitor = _monitor;
        serverList = _monitor->serverList;
        groupList = _monitor->groupList;

        ui();
        connections();

    }

    ~ui_submit(){
        if ( savePanel ) submitPanelSave();
    }

    void ui();
    void connections();

	// Subministration
	void submitDrop( QString path );
	void submitSetPanel( QString file_name );
	void submitPanelOpen();
	void submitAction( QString software );
	void submitPanelSave();
	void submitUpdateBox( int action = 0 );

	//-----------------------------

	// submit
	QComboBox *softwareBox = new QComboBox();
	QLineEdit *projectLine = new QLineEdit();
	QPushButton *projectFile = new QPushButton("Set");
	QLineEdit *renderLine = new QLineEdit();
	QLineEdit *outputLine = new QLineEdit();
	QPushButton *outputPath = new QPushButton("Output");
	QLabel *labelProject = new QLabel("Project:");
	QLabel *labelFile = new QLabel("File:");
	QLabel *labelRender = new QLabel("Render:");
	QLineEdit *fileLine = new QLineEdit();
	QLabel *outputFile = new QLabel("Output:");
	QPushButton *fileFile = new QPushButton("Script");
	QLineEdit *jobName = new QLineEdit();
	combobox *serverBox = new combobox();
	combobox *serverGroupBox = new combobox();
	QComboBox *priority = new QComboBox();
	QLineEdit *taskSize = new QLineEdit();
	QLineEdit *commentLine = new QLineEdit();
	QLineEdit *firstFrame = new QLineEdit();
	QLineEdit *lastFrame = new QLineEdit();
	QCheckBox *suspendBox = new QCheckBox("Suspended");
	QPushButton *buttom = new QPushButton("Submit");

	//--------------------------------------------

	// parameter noice
	QLineEdit *patchRadius = new QLineEdit();
	QLineEdit *searchRadius = new QLineEdit();
	QLineEdit *variance = new QLineEdit();
	QLineEdit *temporalRange = new QLineEdit();
	//------------------------------------

public slots:
	void submitSoftwareBox( int index );

};

#endif //UI_SUBMIT_H