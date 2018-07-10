#include "ui_job_options.h"

ui_job_options::ui_job_options(){
    ui();
}

void ui_job_options::ui(){

	QVBoxLayout *main_vbox = new QVBoxLayout();

		// Titulo
		QLabel *job_label = new QLabel("Jobs Options:");
		main_vbox->addWidget(job_label);
		//-----------------------------------

		// box 1
		QVBoxLayout *box_1_vbox = new QVBoxLayout();
		box_1_vbox->setContentsMargins(15,15,15,15);

		// jobname widget
			QHBoxLayout *hbox1 = new QHBoxLayout();
			hbox1->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *labelJobName = new QLabel("Job Name:");
				labelJobName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				labelJobName->setMinimumWidth(70);

				hbox1->addWidget(labelJobName);
				hbox1->addWidget(jobName); // esta en el .h

			QWidget *widget1 = new QWidget();
			widget1->setLayout(hbox1);
			widget1->setObjectName("style2");
			box_1_vbox->addWidget(widget1);		
		//-------------------------------------

		// priority widget
			QHBoxLayout *hbox4 = new QHBoxLayout();
			hbox4->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *labelPriority = new QLabel("           Priority:");
				labelPriority->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
				labelPriority->setMaximumWidth(70);

				priority->addItems({"Very Low", "Low", "Normal", "High", "Very High"});
				priority->setCurrentIndex(2);
				priority->setMaximumWidth(80);
				
				hbox4->addWidget(labelPriority);
				hbox4->addWidget(priority);
				QLabel *null = new QLabel(" ");
				hbox4->addWidget(null);


			QWidget *widget4 = new QWidget();
			widget4->setLayout(hbox4);
			widget4->setObjectName("style2");

			box_1_vbox->addWidget(widget4);	
		//---------------------------------

		QWidget *box_1_widget = new QWidget();
		box_1_widget->setLayout(box_1_vbox);	
		box_1_widget->setObjectName("style1");

		main_vbox->addWidget(box_1_widget);
		//--------------------------------------------

		// box 2
		QVBoxLayout *box_2_vbox = new QVBoxLayout();
		box_2_vbox->setContentsMargins(15,15,15,15);

		// frame range widget
			QHBoxLayout *hbox2 = new QHBoxLayout();
			hbox2->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *labelFR = new QLabel("Frame Range:");
				labelFR->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
				labelFR->setMinimumWidth(70);

				hbox2->addWidget(labelFR);
				hbox2->addWidget(firstFrame);
				hbox2->addWidget(lastFrame);

			QWidget *widget2 = new QWidget();
			widget2->setLayout(hbox2);
			widget2->setObjectName("style2");
			box_2_vbox->addWidget(widget2);		
		//---------------------------------

		// taskSize widget
			QHBoxLayout *hbox3 = new QHBoxLayout();
			hbox3->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *labelTS = new QLabel("Task Size:");
				labelTS->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				labelTS->setMinimumWidth(70);

				hbox3->addWidget(labelTS);
				hbox3->addWidget(taskSize);

			QWidget *widget3 = new QWidget();
			widget3->setLayout(hbox3);
			widget3->setObjectName("style2");
			box_2_vbox->addWidget(widget3);	
		//---------------------------------

		// instances widget
			QHBoxLayout *hbox5 = new QHBoxLayout();
			hbox5->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *instances_label = new QLabel("Instances:");
				instances_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				instances_label->setMinimumWidth(70);

				hbox5->addWidget(instances_label);
				hbox5->addWidget(instances);

			QWidget *widget5 = new QWidget();
			widget5->setLayout(hbox5);
			widget5->setObjectName("style2");

			box_2_vbox->addWidget(widget5);	
		//---------------------------------

		QWidget *box_2_widget = new QWidget();
		box_2_widget->setLayout(box_2_vbox);	
		box_2_widget->setObjectName("style1");

		main_vbox->addWidget(box_2_widget);
		//--------------------------------------------

		// box 3
		QVBoxLayout *box_3_vbox = new QVBoxLayout();
		box_3_vbox->setContentsMargins(15,15,15,15);

		// comment widget
			QHBoxLayout *hbox6 = new QHBoxLayout();
			hbox6->setContentsMargins(0,0,0,0);
				//------------------------------
				QLabel *labelComment = new QLabel("Comment:");
				labelComment->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				labelComment->setMinimumWidth(70);

				hbox6->addWidget(labelComment);
				hbox6->addWidget(comment);

			QWidget *widget6 = new QWidget();
			widget6->setLayout(hbox6);
			widget6->setObjectName("style2");

			box_3_vbox->addWidget(widget6);		
		//-------------------------------------

		QWidget *box_3_widget = new QWidget();
		box_3_widget->setLayout(box_3_vbox);	
		box_3_widget->setObjectName("style1");

		main_vbox->addWidget(box_3_widget);
		//--------------------------------------------

		// box 4

		QLabel *server_label = new QLabel("Server Asign:");
		main_vbox->addWidget(server_label);

		QVBoxLayout *box_4_vbox = new QVBoxLayout();

		// Server and Groups widget
			QHBoxLayout *hbox7 = new QHBoxLayout();
			hbox7->setContentsMargins(0,0,0,0);
				//------------------------------
				hbox7->addWidget(serverAsign);
				hbox7->addWidget(serverGroupAsign);

			QWidget *widget7 = new QWidget();
			widget7->setLayout(hbox7);

			box_4_vbox->addWidget(widget7);		
		//-------------------------------------

		QWidget *box_4_widget = new QWidget();
		box_4_widget->setLayout(box_4_vbox);

		main_vbox->addWidget(box_4_widget);
		//------------------------

		main_vbox->addWidget(dialog);

	QWidget *main_widget = new QWidget();
	main_widget->setLayout(main_vbox);

	//------------------------------------------

    dialog->setStandardButtons( QDialogButtonBox::Cancel | QDialogButtonBox::Ok );

	this->setObjectName("Job Option");
	this->setWindowTitle("Job Options");
	this->setWidget(main_widget);

	this->hide();
}

