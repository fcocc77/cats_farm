#include "ui_submit.h"

void ui_submit::ui()
{

	QVBoxLayout *vboxSubmit = new QVBoxLayout();
	vboxSubmit->setContentsMargins(15, 15, 15, 15);

	QStringList items = {"Nuke", "Maya", "Houdini", "Cinema4D", "Fusion", "AE"};
	softwareBox->addItems(items);
	vboxSubmit->addWidget(softwareBox);

	// box 1
	QVBoxLayout *vbox2 = new QVBoxLayout();
	vbox2->setContentsMargins(15, 15, 15, 15);

	QHBoxLayout *hbox7 = new QHBoxLayout();
	hbox7->setContentsMargins(0, 0, 0, 0);
	labelProject->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelProject->setMinimumWidth(70);
	projectFile->setMinimumWidth(70);
	//------------------------------
	hbox7->addWidget(labelProject);
	hbox7->addWidget(projectLine);
	hbox7->addWidget(projectFile);
	QWidget *widget7 = new QWidget();
	widget7->setLayout(hbox7);
	widget7->setObjectName("style2");
	vbox2->addWidget(widget7);

	QHBoxLayout *hbox8 = new QHBoxLayout();
	hbox8->setContentsMargins(0, 0, 0, 0);
	labelFile->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
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

	QHBoxLayout *hbox85 = new QHBoxLayout();
	hbox85->setContentsMargins(0, 0, 0, 0);
	outputFile->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	outputFile->setMinimumWidth(70);
	outputPath->setMinimumWidth(70);
	//------------------------------
	hbox85->addWidget(outputFile);
	hbox85->addWidget(outputLine);
	hbox85->addWidget(outputPath);
	QWidget *widget85 = new QWidget();
	widget85->setLayout(hbox85);
	widget85->setObjectName("style2");
	vbox2->addWidget(widget85);

	QHBoxLayout *hbox9 = new QHBoxLayout();
	hbox9->setContentsMargins(0, 0, 0, 0);
	labelRender->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelRender->setMinimumWidth(70);
	//------------------------------
	hbox9->addWidget(labelRender);
	hbox9->addWidget(renderLine);
	QWidget *widget9 = new QWidget();
	widget9->setLayout(hbox9);
	widget9->setObjectName("style2");
	vbox2->addWidget(widget9);

	QWidget *widget2 = new QWidget();
	widget2->setLayout(vbox2);
	widget2->setObjectName("style1");

	vboxSubmit->addWidget(widget2);
	//---------------------------------------

	// box 2
	QVBoxLayout *vbox3 = new QVBoxLayout();
	vbox3->setContentsMargins(15, 15, 15, 15);
	QHBoxLayout *hbox10 = new QHBoxLayout();
	hbox10->setContentsMargins(0, 0, 0, 0);
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

	vboxSubmit->addWidget(widget3);
	//---------------------------------------

	// box 3
	QVBoxLayout *vbox4 = new QVBoxLayout();
	vbox4->setContentsMargins(15, 15, 15, 15);

	QHBoxLayout *hbox11 = new QHBoxLayout();
	hbox11->setContentsMargins(0, 0, 0, 0);

	QLabel *labelFR = new QLabel("Frame Range:");
	labelFR->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelFR->setMinimumWidth(70);

	hbox11->addWidget(labelFR);
	hbox11->addWidget(firstFrame);
	hbox11->addWidget(lastFrame);
	QWidget *widget11 = new QWidget();
	widget11->setLayout(hbox11);
	widget11->setObjectName("style2");
	vbox4->addWidget(widget11);

	QHBoxLayout *hbox12 = new QHBoxLayout();
	hbox12->setContentsMargins(0, 0, 0, 0);

	QLabel *labelTS = new QLabel("Task Size:");
	labelTS->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelTS->setMinimumWidth(70);

	hbox12->addWidget(labelTS);
	hbox12->addWidget(taskSize);
	QWidget *widget12 = new QWidget();
	widget12->setLayout(hbox12);
	widget12->setObjectName("style2");
	vbox4->addWidget(widget12);

	QHBoxLayout *hbox13 = new QHBoxLayout();
	hbox13->setContentsMargins(0, 0, 0, 0);

	QLabel *labelPriority = new QLabel("           Priority:");
	labelPriority->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	labelPriority->setMaximumWidth(70);

	priority->addItems({"Very High", "High", "Normal", "Low", "Very Low"});
	priority->setCurrentIndex(2);
	priority->setMaximumWidth(80);

	hbox13->addWidget(labelPriority);
	hbox13->addWidget(priority);
	QLabel *null = new QLabel(" ");
	hbox13->addWidget(null);
	QWidget *widget13 = new QWidget();
	widget13->setLayout(hbox13);
	widget13->setObjectName("style2");
	vbox4->addWidget(widget13);

	QWidget *widget4 = new QWidget();
	widget4->setLayout(vbox4);
	widget4->setObjectName("style1");

	vboxSubmit->addWidget(widget4);
	//---------------------------------------

	// box 4
	QVBoxLayout *vbox5 = new QVBoxLayout();
	vbox5->setContentsMargins(15, 15, 15, 15);
	QHBoxLayout *hbox14 = new QHBoxLayout();
	hbox14->setContentsMargins(0, 0, 0, 0);
	QLabel *labelSG = new QLabel("Server Group:");
	labelSG->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
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
	hbox16->setContentsMargins(0, 0, 0, 0);
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
	vboxSubmit->addWidget(widget5);
	//--------------------------------------

	// box 5
	QVBoxLayout *vbox6 = new QVBoxLayout();
	vbox6->setContentsMargins(15, 15, 15, 15);
	vbox6->addWidget(suspendBox);

	QWidget *widget6 = new QWidget();
	widget6->setLayout(vbox6);
	widget6->setObjectName("style1");
	vboxSubmit->addWidget(widget6);

	//---------------------------------------
	QWidget *widget1 = new QWidget();
	widget1->setLayout(vboxSubmit);
	widget1->setMaximumHeight(590); //el tamanio maximo del widget

	vboxSubmit->addWidget(buttom);

	//------------------------------------------------

	this->setObjectName("Submit");

	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setWidget(widget1);
	scrollArea->setWidgetResizable(true);
	this->setWindowTitle("Submit");
	this->setWidget(scrollArea);
	this->hide();
}

void ui_submit::connections()
{

	connect(serverBox, &combobox::clicked, this, [this]() { submitUpdateBox(1); });

	connect(serverGroupBox, &combobox::clicked, this, [this]() { submitUpdateBox(2); });

	connect(softwareBox, SIGNAL(activated(int)), this, SLOT(submitSoftwareBox(int)));

	connect(projectFile, &QPushButton::clicked, this, [this]() {
		QString file_path = QFileDialog::getExistingDirectory( monitor ,"Project", projectLine->text() );
		if ( not file_path.isEmpty() ){
			projectLine->setText( file_path );
		} });

	connect(fileFile, &QPushButton::clicked, this, [this]() {
		outputLine->setText("file_name");
		QString project = projectLine->text() + "/scenes";
		QString last_path;

		if ( os::isfile( project ) ) last_path = project;
		else last_path = fileLine->text();

		QString file_name = QFileDialog::getOpenFileName( monitor,"File", last_path );
		if ( not file_name.isEmpty() ){
			submitSetPanel(file_name);

		} });

	connect(outputPath, &QPushButton::clicked, this, [this]() {
		QString Output = outputLine->text() + "/scenes";
		QString last_path;

		if ( os::isfile( Output )) last_path = Output;
		else last_path = outputLine->text();

		QString file_name = QFileDialog::getSaveFileName(monitor, "File", last_path );
		if ( not file_name.isEmpty() ){
			outputLine->setText(file_name);
		} });

	connect(buttom, &QPushButton::clicked, this, [this]() {
		submitAction(softwareBox->currentText());
	});
	//------------------------
}

// Subministration
void ui_submit::submitDrop(QString path)
{
	this->show();

	submitSetPanel(path);
}

void ui_submit::submitSoftwareBox(int index = 0)
{

	QString software = softwareBox->currentText();

	if (software == "Nuke")
	{
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

	if (software == "Maya")
	{
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

		QString file_name = fileLine->text();

		for (int i = 0; i < 10; i++)
		{
			file_name = os::dirname(file_name);
			if (os::listdir(file_name, true).contains("workspace.mel"))
				break;
		}

		projectLine->setText(file_name);
	}

	if (software == "Houdini")
	{
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
	if (software == "Cinema4D")
	{
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

	if (software == "Fusion")
	{
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

	if (software == "AE")
	{
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
}

void ui_submit::submitSetPanel(QString file_name)
{
	QString file = file_name;

	file = file.replace("\\", "/");
	QString ext = file.split(".").last();
	QString name = os::basename(file).replace("." + ext, "");

	if ((ext == "mb") or (ext == "ma"))
		softwareBox->setCurrentIndex(1);
	if (ext == "nk")
		softwareBox->setCurrentIndex(0);
	if (ext == "c4d")
		softwareBox->setCurrentIndex(3);
	if (ext == "hip")
		softwareBox->setCurrentIndex(2);
	if (ext == "comp")
		softwareBox->setCurrentIndex(4);
	if (ext == "ntp")
		softwareBox->setCurrentIndex(5);

	fileLine->setText(file);
	jobName->setText(name);

	submitSoftwareBox();
}

void ui_submit::submitUpdateBox(int action)
{

	// server
	if ((action == 0) or (action == 1))
	{

		int index = serverBox->currentIndex();
		serverBox->clear();
		serverBox->addItems({"None"});

		for (int i = 0; i < serverList->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *item = serverList->topLevelItem(i);
			serverBox->addItems({item->text(0)});
		}

		if (action)
			serverBox->setCurrentIndex(index);
	}
	//-------------------------------------------------

	// server group
	if ((action == 0) or (action == 2))
	{

		int index = serverGroupBox->currentIndex();
		serverGroupBox->clear();
		serverGroupBox->addItems({"None"});

		for (int i = 0; i < groupList->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *item = groupList->topLevelItem(i);
			serverGroupBox->addItems({item->text(2)});
		}

		if (action)
			serverGroupBox->setCurrentIndex(index);
	}
	//-----------------------------------------------
}

void ui_submit::submitPanelOpen()
{

	QJsonObject panel = jread(path + "/etc/panel.json");

	if (not panel.empty())
	{
		projectLine->setText(panel["projectLine"].toString());

		fileLine->setText(panel["fileLine"].toString());
		outputLine->setText(panel["outputLine"].toString());
		renderLine->setText(panel["renderLine"].toString());
		softwareBox->setCurrentIndex(panel["softwareBox"].toInt());
		jobName->setText(panel["jobName"].toString());
		firstFrame->setText(panel["firstFrame"].toString());
		lastFrame->setText(panel["lastFrame"].toString());
		taskSize->setText(panel["taskSize"].toString());
		priority->setCurrentIndex(panel["priority"].toInt());
		serverBox->setCurrentIndex(panel["serverBox"].toInt());
		serverGroupBox->setCurrentIndex(panel["serverGroupBox"].toInt());
		commentLine->setText(panel["commentLine"].toString());
		suspendBox->setChecked(panel["suspendBox"].toInt());
	}
}

void ui_submit::submitPanelSave()
{

	QJsonObject panel = {
		{"projectLine", projectLine->text()},
		{"fileLine", fileLine->text()},
		{"outputLine", outputLine->text()},
		{"renderLine", renderLine->text()},
		{"jobName", jobName->text()},
		{"firstFrame", firstFrame->text()},
		{"lastFrame", lastFrame->text()},
		{"taskSize", taskSize->text()},
		{"priority", priority->currentIndex()},
		{"serverBox", serverBox->currentIndex()},
		{"serverGroupBox", serverGroupBox->currentIndex()},
		{"softwareBox", softwareBox->currentIndex()},
		{"commentLine", commentLine->text()},
		{"suspendBox", suspendBox->isChecked()},
	};

	jwrite(path + "/etc/panel.json", panel);
}

void ui_submit::submitAction(QString software)
{

	bool suspend = false;
	if (suspendBox->isChecked())
		suspend = true;

	QString dirProject = projectLine->text();
	if (software == "Fusion")
		dirProject = outputLine->text();

	QString system;
	if (_linux)
		system = "Linux";
	else if (_win32)
		system = "Windows";
	else
		system = "Mac";

	// crea lista de texto facil para poder enviar por tcp.
	QJsonArray info = {jobName->text(),
					   serverBox->currentText(),
					   serverGroupBox->currentText(),
					   firstFrame->text().toInt(),
					   lastFrame->text().toInt(),
					   taskSize->text().toInt(),
					   priority->currentIndex(),
					   suspend,
					   commentLine->text(),
					   software,
					   fileLine->text(),
					   dirProject,
					   system,
					   1,
					   renderLine->text()};
	//------------------------------------------------------

	bool ok = true;
	QString details = "Incomplete Fiels:\n";

	if (jobName->text().isEmpty())
	{
		details += "Job Name\n";
		ok = false;
	}
	if (firstFrame->text().isEmpty())
	{
		details += "First Frame\n";
		ok = false;
	}
	if (lastFrame->text().isEmpty())
	{
		details += "Last Frame\n";
		ok = false;
	}
	if (taskSize->text().isEmpty())
	{
		details += "Task Size\n";
		ok = false;
	}

	QMessageBox *msg = new QMessageBox(this);
	msg->setWindowTitle("Submit Information");

	if (ok)
	{
		submitPanelSave();
		const QString managerHost = fread(path + "/etc/manager_host");

		tcpClient(managerHost, 7000, jats({4, info}));
		msg->setText("The " + jobName->text() + " job has sended.");
		msg->show();
	}
	else
	{
		msg->setIcon(QMessageBox::Information);
		msg->setText("You must fill in all the boxes.");
		msg->setDetailedText(details);
		msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msg->show();
	}
}
//----------------------------------------------
