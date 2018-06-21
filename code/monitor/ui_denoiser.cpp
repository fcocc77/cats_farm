#include "ui_denoiser.h"

void ui_denoiser::ui(){

	QVBoxLayout *vbox1 = new QVBoxLayout();
	vbox1->setContentsMargins(15,15,15,15);

	    // box 1
		QVBoxLayout *vbox2 = new QVBoxLayout();
		vbox2->setContentsMargins(15,15,15,15);

			QHBoxLayout *hbox8 = new QHBoxLayout();
			hbox8->setContentsMargins(0,0,0,0);
				labelFile->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				labelFile->setMinimumWidth(70);
				fileFile->setMinimumWidth(70);
				//------------------------------
				hbox8->addWidget(labelFile);
				hbox8->addWidget(fileLine);
				hbox8->addWidget(fileFile);
			QWidget *widget8 = new QWidget();
			widget8->setLayout(hbox8);	
			widget8->setObjectName("style2");
			vbox2->addWidget(widget8);

			QHBoxLayout *hbox8_2 = new QHBoxLayout();
			hbox8_2->setContentsMargins(0,0,0,0);

				QLabel *patchRadius_label = new QLabel("Patch Radius:");
				patchRadius_label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				patchRadius_label->setMinimumWidth(70);
				//------------------------------
				hbox8_2->addWidget(patchRadius_label);
				hbox8_2->addWidget(patchRadius);
			QWidget *widget8_2 = new QWidget();
			widget8_2->setLayout(hbox8_2);	
			widget8_2->setObjectName("style2");
			vbox2->addWidget(widget8_2);

			QHBoxLayout *hbox8_3 = new QHBoxLayout();
			hbox8_3->setContentsMargins(0,0,0,0);

				QLabel *searchRadius_label = new QLabel("Search Radius:");
				searchRadius_label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				searchRadius_label->setMinimumWidth(70);
				//------------------------------
				hbox8_3->addWidget(searchRadius_label);
				hbox8_3->addWidget(searchRadius);
			QWidget *widget8_3 = new QWidget();
			widget8_3->setLayout(hbox8_3);	
			widget8_3->setObjectName("style2");
			vbox2->addWidget(widget8_3);

			QHBoxLayout *hbox8_4 = new QHBoxLayout();
			hbox8_4->setContentsMargins(0,0,0,0);

				QLabel *variance_label = new QLabel("Variance:");
				variance_label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				variance_label->setMinimumWidth(70);
				//------------------------------
				hbox8_4->addWidget(variance_label);
				hbox8_4->addWidget(variance);
			QWidget *widget8_4 = new QWidget();
			widget8_4->setLayout(hbox8_4);	
			widget8_4->setObjectName("style2");
			vbox2->addWidget(widget8_4);

			QHBoxLayout *hbox8_5 = new QHBoxLayout();
			hbox8_5->setContentsMargins(0,0,0,0);

				QLabel *temporalRange_label = new QLabel("Temporal Range:");
				temporalRange_label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				temporalRange_label->setMinimumWidth(70);
				//------------------------------
				hbox8_5->addWidget(temporalRange_label);
				hbox8_5->addWidget(temporalRange);
			QWidget *widget8_5 = new QWidget();
			widget8_5->setLayout(hbox8_5);	
			widget8_5->setObjectName("style2");
			vbox2->addWidget(widget8_5);

		QWidget *widget2 = new QWidget();
		widget2->setLayout(vbox2);	
		widget2->setObjectName("style1");

		vbox1->addWidget(widget2);
		//---------------------------------------

	    // box 2
		QVBoxLayout *vbox3 = new QVBoxLayout();
		vbox3->setContentsMargins(15,15,15,15);

			QHBoxLayout *hbox10 = new QHBoxLayout();
			hbox10->setContentsMargins(0,0,0,0);
				QLabel *labelJobName = new QLabel("Job Name:");
				labelJobName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				labelJobName->setMinimumWidth(70);
				//---------------------------------------
				hbox10->addWidget(labelJobName);
				hbox10->addWidget(jobName);
			QWidget *widget10 = new QWidget();
			widget10->setLayout(hbox10);	
			widget10->setObjectName("style2");

			vbox3->addWidget(widget10);

		QWidget *widget3 = new QWidget();
		widget3->setLayout(vbox3);	
		widget3->setObjectName("style1");

		vbox1->addWidget(widget3);
		//---------------------------------------

		// box 3
		QVBoxLayout *vbox4 = new QVBoxLayout();
		vbox4->setContentsMargins(15,15,15,15);

			QHBoxLayout *hbox11 = new QHBoxLayout();
			hbox11->setContentsMargins(0,0,0,0);

				QLabel *labelFR = new QLabel("Frame Range:");
				labelFR->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
				labelFR->setMinimumWidth(70);

				hbox11->addWidget(labelFR);
				hbox11->addWidget(firstFrame);
				hbox11->addWidget(lastFrame);
			QWidget *widget11 = new QWidget();
			widget11->setLayout(hbox11);	
			widget11->setObjectName("style2");
			vbox4->addWidget(widget11);

			QHBoxLayout *hbox12 = new QHBoxLayout();
			hbox12->setContentsMargins(0,0,0,0);

				QLabel *labelTS = new QLabel("Task Size:");
				labelTS->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
				labelTS->setMinimumWidth(70);

				hbox12->addWidget(labelTS);
				hbox12->addWidget(taskSize);
			QWidget *widget12 = new QWidget();
			widget12->setLayout(hbox12);	
			widget12->setObjectName("style2");
			vbox4->addWidget(widget12);

			QHBoxLayout *hbox13 = new QHBoxLayout();
			hbox13->setContentsMargins(0,0,0,0);

				QLabel *labelPriority = new QLabel("Priority:");
				labelPriority->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				labelPriority->setMinimumWidth(70);

				hbox13->addWidget(labelPriority);
				hbox13->addWidget(priority);
			QWidget *widget13 = new QWidget();
			widget13->setLayout(hbox13);	
			widget13->setObjectName("style2");
			vbox4->addWidget(widget13);

		QWidget *widget4 = new QWidget();
		widget4->setLayout(vbox4);	
		widget4->setObjectName("style1");

		vbox1->addWidget(widget4);
		//---------------------------------------

		// box 4
		QVBoxLayout *vbox5 = new QVBoxLayout();
		vbox5->setContentsMargins(15,15,15,15);
			QHBoxLayout *hbox14 = new QHBoxLayout();
			hbox14->setContentsMargins(0,0,0,0);
				QLabel *labelSG = new QLabel("Server Group:");
				labelSG->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
				labelSG->setMinimumWidth(70);

				QLabel *labelServer = new QLabel("Server:");
				labelServer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

				hbox14->addWidget(labelSG);
				hbox14->addWidget(serverGroupBox);
				hbox14->addWidget(labelServer);
				hbox14->addWidget(serverBox);
			QWidget *widget14 = new QWidget();
			widget14->setLayout(hbox14);	
			widget14->setObjectName("style2");
			vbox5->addWidget(widget14);

			QHBoxLayout *hbox16 = new QHBoxLayout();
			hbox16->setContentsMargins(0,0,0,0);
				QLabel *labelComment = new QLabel("Comment:");
				labelComment->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
				labelComment->setMinimumWidth(70);
				hbox16->addWidget(labelComment);
				hbox16->addWidget(commentLine);
			QWidget *widget16 = new QWidget();
			widget16->setLayout(hbox16);	
			widget16->setObjectName("style2");
			vbox5->addWidget(widget16);
		QWidget *widget5 = new QWidget();
		widget5->setLayout(vbox5);	
		widget5->setObjectName("style1");
		vbox1->addWidget(widget5);
		//--------------------------------------

		// box 5
		QVBoxLayout *vbox6 = new QVBoxLayout();
		vbox6->setContentsMargins(15,15,15,15);
			vbox6->addWidget(suspendBox);

		QWidget *widget6 = new QWidget();
		widget6->setLayout(vbox6);	
		widget6->setObjectName("style1");
		vbox1->addWidget(widget6);

		//---------------------------------------

	QWidget *widget1 = new QWidget();
	widget1->setLayout(vbox1);
    widget1->setMaximumHeight(500); //el tamanio maximo del widget

	vbox1->addWidget(buttom);

	//------------------------------------------------

	this->setObjectName("Denoiser");

	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setWidget(widget1);
	scrollArea->setWidgetResizable(true);
	this->setWindowTitle("Arnold Denoiser");
	this->setWidget(scrollArea);
	this->hide();
}

void ui_denoiser::connections(){

	connect( serverBox, &qcombobox::clicked, this, [this](){ submitUpdateBox(1); });

	connect( serverGroupBox, &qcombobox::clicked, this, [this](){ submitUpdateBox(2); });

	connect( softwareBox, SIGNAL( activated(int) ), this, SLOT( submitSoftwareBox(int) ) );

	connect( projectFile, &QPushButton::clicked, this, [this] (){
		QString file_path = QFileDialog::getExistingDirectory( monitor ,"Project", projectLine->text() );
		if ( not file_path.isEmpty() ){
			projectLine->setText( file_path );
		}}
	);

	connect( fileFile, &QPushButton::clicked, this, [this] (){
		outputLine->setText("file_name");
		QString project = projectLine->text() + "/scenes";
		QString last_path;

		if ( os::isfile( project.toStdString() )){ last_path = project; }
		else{ last_path = fileLine->text(); }

		QString file_name = QFileDialog::getOpenFileName( monitor,"File", last_path );
		if ( not file_name.isEmpty() ){
			submitSetPanel(file_name);

		}}
	);

	connect( outputPath, &QPushButton::clicked, this, [this] (){
		QString Output = outputLine->text() + "/scenes";
		QString last_path;

		if ( os::isfile( Output.toStdString() )){ last_path = Output; }
		else{ last_path = outputLine->text(); }

		QString file_name = QFileDialog::getSaveFileName(monitor, "File", last_path );
		if ( not file_name.isEmpty() ){
			outputLine->setText(file_name);
		}}
	);

	connect( buttom, &QPushButton::clicked, this, [this] (){
		submitAction( softwareBox->currentText() );
	});	
	//------------------------
}

// Subministration
void ui_denoiser::submitDrop( QString path ){
	this->show();

	submitSetPanel( path);
}

void ui_denoiser::submitSoftwareBox( int index = 0 ){

	QString software = softwareBox->currentText();

	if ( software == "Nuke" ){
		projectLine->setDisabled(1);
		projectFile->setDisabled(1);
		renderLine->setDisabled(0);
		outputLine->setDisabled(1);
		outputPath->setDisabled(1);

		projectLine->setText("");
		labelProject->setText("...");
		labelFile->setText("Script:");
		labelRender->setText("Write:");
		fileFile->setText("Script");
		renderLine->setText("Write1");
		outputFile->setText("...");
	}

	if ( software == "Maya" ){
		projectLine->setDisabled(0);
		projectFile->setDisabled(0);
		renderLine->setDisabled(1);
		outputLine->setDisabled(1);
		outputPath->setDisabled(1);

		labelProject->setText("Project");
		labelFile->setText("Scene:");
		labelRender->setText("...");
		fileFile->setText("Scene");
		renderLine->setText("");
		outputFile->setText("...");

		string file_name = ( fileLine->text() ).toStdString();

		for ( int i=0; i<10; i++ ){
			file_name = os::dirname( file_name );
			if ( in_vector( "workspace.mel", os::listdir( file_name, true ) ) ){
				break;
			}
		}

		projectLine->setText( QString::fromStdString(file_name) );
	}

	if ( software == "Houdini" ){
		projectLine->setDisabled(1);
		projectFile->setDisabled(1);
		renderLine->setDisabled(0);
		outputLine->setDisabled(1);
		outputPath->setDisabled(1);

		fileFile->setText("Project");
		labelProject->setText("...");
		labelFile->setText("Project:");
		labelRender->setText("Engine:");
		renderLine->setText("/out/arnold1");
		projectLine->setText("");
		outputFile->setText("...");
	}
	if ( software == "Cinema4D" ){
		projectLine->setDisabled(1);
		projectFile->setDisabled(1);
		renderLine->setDisabled(1);
		outputLine->setDisabled(1);
		outputPath->setDisabled(1);

		fileFile->setText("Project");
		labelProject->setText("...");
		labelFile->setText("Project:");
		labelRender->setText("...");
		renderLine->setText("");
		projectLine->setText("");
		outputFile->setText("...");
	}

	if ( software == "Fusion" ){
		projectLine->setDisabled(1);
		projectFile->setDisabled(1);
		renderLine->setDisabled(1);
		outputLine->setDisabled(1);
		outputPath->setDisabled(1);

		fileFile->setText("Project");
		labelProject->setText("...");
		labelFile->setText("Project:");
		labelRender->setText("...");
		renderLine->setText("");
		projectLine->setText("");
		outputFile->setText("...");
	}
}

void ui_denoiser::submitSetPanel( QString file_name ){
	string file = file_name.toStdString();

	file = replace( file, "\\", "/" );
	string ext = split( file, "." ).back();
	string name = replace( os::basename( file ), "."+ext, "" );

	if ( ( ext == "mb" ) or ( ext == "ma" ) ){
		softwareBox->setCurrentIndex(1);
	}
	if ( ext == "nk" ){
		softwareBox->setCurrentIndex(0);
	}

	if ( ext == "c4d" ){
		softwareBox->setCurrentIndex(3);
	}
	if ( ext == "hip" ){
		softwareBox->setCurrentIndex(2);
	}
	if ( ext == "comp" ){
		softwareBox->setCurrentIndex(4);
	}

	fileLine->setText( QString::fromStdString( file ) );
	jobName->setText( QString::fromStdString ( name ) );

	submitSoftwareBox();	
}

void ui_denoiser::submitUpdateBox( int action ){

	// server
	if ( ( action == 0 ) or ( action == 1 ) ){

		int index = serverBox->currentIndex();
		serverBox->clear();
		serverBox->addItems({"None"});

		for( int i = 0; i < serverList->topLevelItemCount(); ++i ){
			QTreeWidgetItem *item = serverList->topLevelItem( i );
			serverBox->addItems( { item->text(0) } );
		}

		if ( action ) serverBox->setCurrentIndex( index );

	}
	//-------------------------------------------------

	// server group
	if ( ( action == 0 ) or ( action == 2 ) ){

		int index = serverGroupBox->currentIndex();
		serverGroupBox->clear();
		serverGroupBox->addItems({"None"});

		for( int i = 0; i < groupList->topLevelItemCount(); ++i ){
			QTreeWidgetItem *item = groupList->topLevelItem( i );
			serverGroupBox->addItems( { item->text(2) } );
		}

		if ( action ) serverGroupBox->setCurrentIndex( index );
	}
	//-----------------------------------------------
}

void ui_denoiser::submitPanelOpen(){

	json panel = jread( "../../etc/panel.json" );

	if ( not panel.empty() ){
		projectLine->setText( QString::fromStdString( panel["projectLine"] ) );

		fileLine->setText( QString::fromStdString(panel["fileLine"]) );	
		outputLine->setText( QString::fromStdString(panel["outputLine"]) );
		renderLine->setText( QString::fromStdString(panel["renderLine"]) );
		softwareBox->setCurrentIndex( panel["softwareBox"] );
		jobName->setText( QString::fromStdString( panel["jobName"]) );	
		firstFrame->setText( QString::fromStdString( panel["firstFrame"]) );	
		lastFrame->setText( QString::fromStdString( panel["lastFrame"]) );
		taskSize->setText( QString::fromStdString( panel["taskSize"]) );	
		priority->setText( QString::fromStdString( panel["priority"]) );	
		serverBox->setCurrentIndex( int( panel["serverBox"] ) );
		serverGroupBox->setCurrentIndex( int( panel["serverGroupBox"] ) );
		commentLine->setText( QString::fromStdString( panel["commentLine"]) );
		suspendBox->setChecked( panel["suspendBox"] );

    }	
}

void ui_denoiser::submitPanelSave(){

	json panel = {  { "projectLine", projectLine->text().toStdString() },
				    { "fileLine", fileLine->text().toStdString() },
				    { "outputLine", outputLine->text().toStdString() },
				    { "renderLine", renderLine->text().toStdString() },
				    { "jobName", jobName->text().toStdString() },
				    { "firstFrame", firstFrame->text().toStdString() },
				    { "lastFrame", lastFrame->text().toStdString() },
				    { "taskSize", taskSize->text().toStdString()  },
				    { "priority", priority->text().toStdString() },
				    { "serverBox",  serverBox->currentIndex() },
				    { "serverGroupBox", serverGroupBox->currentIndex() },
				    { "softwareBox", softwareBox->currentIndex() },
				    { "commentLine", commentLine->text().toStdString() },
				    { "suspendBox", suspendBox->isChecked() }, 
				};

	jwrite( "../../etc/panel.json", panel );
}

void ui_denoiser::submitAction( QString software ){

	bool suspend = false;
	if ( suspendBox->isChecked() ){ suspend = true; }

    QString dirProject = projectLine->text();
	if ( software == "Fusion" ){ dirProject = outputLine->text(); }

	string system;
	if ( _linux ){ system = "Linux"; }
	else if ( _win32 ){ system = "Windows"; }
	else { system = "Mac"; }

	// crea lista de texto facil para poder enviar por tcp.
	json info = {   jobName->text().toStdString(),
		            serverBox->currentText().toStdString(),
		            serverGroupBox->currentText().toStdString(),
		            firstFrame->text().toInt(),
		            lastFrame->text().toInt(),
		            taskSize->text().toInt(),
		            priority->text().toInt(),
		            suspend,
		            commentLine->text().toStdString(),
		            software.toStdString(),
		            fileLine->text().toStdString(),
		            dirProject.toStdString(),
		            system,
		            1,
		            renderLine->text().toStdString()
		        };
	//------------------------------------------------------

	bool ok = true;
	QString details = "Incomplete Fiels:\n";

	if ( jobName->text().isEmpty() ) { details += "Job Name\n"; ok = false; }
	if ( firstFrame->text().isEmpty() ) { details += "First Frame\n"; ok = false; }
	if ( lastFrame->text().isEmpty() ) { details += "Last Frame\n"; ok = false; }
	if ( taskSize->text().isEmpty() ) { details += "Task Size\n"; ok = false; }
	if ( priority->text().isEmpty() ) { details += "Priority\n"; ok = false; }

	QMessageBox *msg = new QMessageBox( this );
	msg->setWindowTitle("Submit Information");

	if ( ok ){
		submitPanelSave();
        const string managerHost = fread( "../../etc/manager_host" );

		tcpClient( managerHost, 7000, info, 0 );
		msg->setText("The " + jobName->text() + " job has sended." );
		msg->show();
	}
	else{
		msg->setIcon( QMessageBox::Information );
		msg->setText( "You must fill in all the boxes." );
		msg->setDetailedText( details );
		msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
		msg->show();
	}
}
//----------------------------------------------

