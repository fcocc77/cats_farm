#ifndef UI_SERVER_OPTIONS
#define UI_SERVER_OPTIONS

// QT5 Library
#include <QDockWidget>
#include <QComboBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
//-----------


// Utils Library
#include "../utils/os.h"

//----------


class ui_server_options : public QDockWidget{
public:

    ui_server_options();
   
    void ui();

	//server_option_ui
	QComboBox *schedule_start = new QComboBox();
	QComboBox *schedule_end = new QComboBox();
	//------------------

};












#endif //UI_SERVER_OPTIONS