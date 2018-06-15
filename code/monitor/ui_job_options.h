#ifndef UI_JOB_OPTIONS
#define UI_JOB_OPTIONS

// QT5 Library
#include <QDockWidget>
#include <QLineEdit>
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
//-----------


// Utils Library
#include "../utils/os.h"

//----------


class ui_job_options : public QDockWidget{
public:

    ui_job_options();
   
    void ui();

    //jobs
	QLineEdit *jobName = new QLineEdit();
	
	QLineEdit *firstFrame = new QLineEdit();
	QLineEdit *lastFrame = new QLineEdit();
	QLineEdit *taskSize = new QLineEdit();
	QLineEdit *priority = new QLineEdit();
	QLineEdit *instances = new QLineEdit();

	QCheckBox *priority0 = new QCheckBox("Very Low");
	QCheckBox *priority1 = new QCheckBox("Low");
	QCheckBox *priority2 = new QCheckBox("Normal");
	QCheckBox *priority3 = new QCheckBox("High");
	QCheckBox *priority4 = new QCheckBox("Very High");

	QLineEdit *comment = new QLineEdit();

	QTreeWidget *serverAsign = new QTreeWidget();
	QTreeWidget *serverGroupAsign = new QTreeWidget();
	QDialogButtonBox *dialog = new QDialogButtonBox();
	//-----------------------------------------------

};












#endif //UI_JOB_OPTIONS