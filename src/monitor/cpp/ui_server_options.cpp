#include "../hpp/ui_server_options.h"

ui_server_options::ui_server_options()
{
	ui();
}

void ui_server_options::ui()
{

	//botonera
	QHBoxLayout *hbox = new QHBoxLayout();
	QWidget *button_widget = new QWidget();
	button_widget->setLayout(hbox);

	QPushButton *ok = new QPushButton("OK");
	QPushButton *ap = new QPushButton("Apply");
	QPushButton *cancel = new QPushButton("Cancel");

	connect(cancel, &QPushButton::clicked, this, [this]() {
		this->hide();
	});
	connect(ap, &QPushButton::clicked, this, [this]() {
		//serverOptionsSend();
	});
	connect(ok, &QPushButton::clicked, this, [this]() {
		//serverOptionsSend();
		this->hide();
	});

	hbox->addWidget(cancel);
	hbox->addWidget(ap);
	hbox->addWidget(ok);
	//------------------------------

	//chedule widget
	QHBoxLayout *schedule_Layout = new QHBoxLayout();
	QWidget *schedule_widget = new QWidget();

	QLabel *schedule_label = new QLabel("Range of time the server is active:");
	schedule_label->setAlignment(Qt::AlignCenter);
	QLabel *schedule_label2 = new QLabel("-----------------------------------------------------------");
	schedule_label2->setAlignment(Qt::AlignCenter);

	for (int i = 0; i < 24; i++)
	{
		schedule_start->addItem(QString::number(i) + ":00");
		schedule_end->addItem(QString::number(i) + ":00");
	}

	QLabel *startLabel = new QLabel("Start Time:");
	startLabel->setAlignment(Qt::AlignRight);

	QLabel *endLabel = new QLabel("End Time:");
	endLabel->setAlignment(Qt::AlignRight);

	schedule_Layout->addWidget(startLabel);
	schedule_Layout->addWidget(schedule_start);
	schedule_Layout->addWidget(endLabel);

	schedule_Layout->addWidget(schedule_end);
	schedule_widget->setLayout(schedule_Layout);

	//---------------------

	QWidget *widget = new QWidget();
	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget(schedule_label);
	vbox->addWidget(schedule_widget);
	vbox->addWidget(schedule_label2);
	vbox->addWidget(button_widget);

	widget->setLayout(vbox);

	this->setObjectName("Server Option");
	this->setWidget(widget);
	this->hide();
	this->setWindowTitle("Server Options");
}