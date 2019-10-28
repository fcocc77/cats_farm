#include "../hpp/settings.h"

void settings::ui()
{

	QVBoxLayout *layout_ui = new QVBoxLayout();
	layout_ui->setContentsMargins(20, 20, 20, 20);
	QWidget *widget = new QWidget();
	widget->setLayout(layout_ui);
	this->setCentralWidget(widget);

	//botonera
	QHBoxLayout *hbox = new QHBoxLayout();
	QWidget *button_widget = new QWidget();
	button_widget->setLayout(hbox);

	QPushButton *ok = new QPushButton("OK");
	QPushButton *ap = new QPushButton("Apply");
	QPushButton *cancel = new QPushButton("Cancel");

	connect(cancel, &QPushButton::clicked, this, [this]() { close(); });
	connect(ap, &QPushButton::clicked, this, [this]() { pathSusWrite(); });
	connect(ok, &QPushButton::clicked, this, [this]() {
		QString _manager_ip = manager_ip->text();
		fwrite(path + "/etc/manager_host", _manager_ip);

		pathSusWrite();

		close();
	});

	hbox->addWidget(cancel);
	hbox->addWidget(ap);
	hbox->addWidget(ok);

	//------------------------------
	layout_ui->addWidget(general_ui());
	layout_ui->addWidget(tab_widget);
	layout_ui->addWidget(button_widget);

	path_ui();

	tab_widget->setCurrentIndex(0);
}

void settings::style()
{

	// centrar ventana
	this->setGeometry(
		QStyle::alignedRect(
			Qt::LeftToRight,
			Qt::AlignCenter,
			monitor->size(),
			qApp->desktop()->availableGeometry()));
	//-------------------------------------

	// mantiene la ventana por encima de todo
	Qt::WindowFlags flags = windowFlags();
	this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
	//---------------------------------------------------

	resize(700, 400);
	QString _style = fread(path + "/theme/style.css");
	setStyleSheet(_style.toStdString().c_str());
	setWindowTitle("CatsFarm Setting");

	// Agrega el icono y titulo al panel y ventana
	QIcon *app_icon = new QIcon();
	app_icon->addFile(path + "/icons/app.ico", QSize(16, 16));
	//setWindowIcon( app_icon );
	//---------------------------------------------------
}

QWidget *settings::general_ui()
{
	// Path----------------------------
	QWidget *widget = new QWidget();
	widget->setObjectName("style1");
	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->setContentsMargins(50, 20, 50, 20);
	QLabel *manager_label = new QLabel("Manager IP:");

	hbox->addWidget(manager_label);
	hbox->addWidget(manager_ip);

	widget->setLayout(hbox);

	//----------------------------

	return widget;
}

void settings::path_ui()
{
	int size_plain = 50;

	// SystemPath----------------------------
	QWidget *widget1 = new QWidget();
	QVBoxLayout *vbox1 = new QVBoxLayout();

	QLabel *label1 = new QLabel("System Path:");

	vbox1->addWidget(label1);
	vbox1->addWidget(system_edit);

	widget1->setLayout(vbox1);
	widget1->setMinimumHeight(size_plain);

	system_edit->setObjectName("style1");
	tab_widget->addTab(widget1, "Paths");
	//--------------------------------------

	// Maya Path----------------------------
	QWidget *widget2 = new QWidget();
	QVBoxLayout *vbox2 = new QVBoxLayout();

	QLabel *label2 = new QLabel("Maya Path:");

	vbox2->addWidget(label2);
	vbox2->addWidget(maya_edit);

	widget2->setLayout(vbox2);
	widget2->setMinimumHeight(size_plain);

	maya_edit->setObjectName("style1");
	tab_widget->addTab(widget2, "Maya");
	//--------------------------------------

	// Nuke Path----------------------------
	QWidget *widget3 = new QWidget();
	QVBoxLayout *vbox3 = new QVBoxLayout();

	QLabel *label3 = new QLabel("Nuke Path:");

	vbox3->addWidget(label3);
	vbox3->addWidget(nuke_edit);

	widget3->setLayout(vbox3);
	widget3->setMinimumHeight(size_plain);

	nuke_edit->setObjectName("style1");
	tab_widget->addTab(widget3, "Nuke");
	//--------------------------------------

	// Houdini Path----------------------------
	QWidget *widget4 = new QWidget();
	QVBoxLayout *vbox4 = new QVBoxLayout();

	QLabel *label4 = new QLabel("Houdini Path:");

	vbox4->addWidget(label4);
	vbox4->addWidget(houdini_edit);

	widget4->setLayout(vbox4);
	widget4->setMinimumHeight(size_plain);

	houdini_edit->setObjectName("style1");
	tab_widget->addTab(widget4, "Houdini");
	//--------------------------------------

	// Cinema Path----------------------------
	QWidget *widget5 = new QWidget();
	QVBoxLayout *vbox5 = new QVBoxLayout();

	QLabel *label5 = new QLabel("Cinema 4D Path:");

	vbox5->addWidget(label5);
	vbox5->addWidget(cinema_edit);

	widget5->setLayout(vbox5);
	widget5->setMinimumHeight(size_plain);

	cinema_edit->setObjectName("style1");
	tab_widget->addTab(widget5, "Cinema 4D");
	//--------------------------------------

	// Fusion Path----------------------------
	QWidget *widget6 = new QWidget();
	QVBoxLayout *vbox6 = new QVBoxLayout();

	QLabel *label6 = new QLabel("Fusion Path:");

	vbox6->addWidget(label6);
	vbox6->addWidget(fusion_edit);

	widget6->setLayout(vbox6);
	widget6->setMinimumHeight(size_plain);

	fusion_edit->setObjectName("style1");
	tab_widget->addTab(widget6, "Fusion");
	//--------------------------------------

	// AE Path----------------------------
	QWidget *widget7 = new QWidget();
	QVBoxLayout *vbox7 = new QVBoxLayout();

	QLabel *label7 = new QLabel("AE Path:");

	vbox7->addWidget(label7);
	vbox7->addWidget(ae_edit);

	widget7->setLayout(vbox7);
	widget7->setMinimumHeight(size_plain);

	ae_edit->setObjectName("style1");
	tab_widget->addTab(widget7, "AE");
	//--------------------------------------
}

void settings::pathSusRead()
{

	QJsonArray send = {"preferences", QJsonArray({"read", "none"})};
	QString recv = tcpClient(managerHost, 7000, jats({3, send}));
	QJsonObject preferences = jofs(recv);

	if (not preferences.empty())
	{
		QJsonObject paths = preferences["paths"].toObject();

		QString system, nuke, maya, houdini, cinema, fusion, ae;

		for (QJsonValue p : paths["system"].toArray())
		{
			system += p.toString() + "\n";
		}
		for (QJsonValue p : paths["nuke"].toArray())
		{
			nuke += p.toString() + "\n";
		}
		for (QJsonValue p : paths["houdini"].toArray())
		{
			houdini += p.toString() + "\n";
		}
		for (QJsonValue p : paths["cinema"].toArray())
		{
			cinema += p.toString() + "\n";
		}
		for (QJsonValue p : paths["fusion"].toArray())
		{
			fusion += p.toString() + "\n";
		}
		for (QJsonValue p : paths["maya"].toArray())
		{
			maya += p.toString() + "\n";
		}

		for (QJsonValue p : paths["ae"].toArray())
		{
			ae += p.toString() + "\n";
		}

		system_edit->setPlainText(system);
		nuke_edit->setPlainText(nuke);
		maya_edit->setPlainText(maya);
		houdini_edit->setPlainText(houdini);
		cinema_edit->setPlainText(cinema);
		fusion_edit->setPlainText(fusion);
		ae_edit->setPlainText(ae);
	}
}

void settings::pathSusWrite()
{

	QJsonObject paths;

	QJsonArray system;
	for (auto l : system_edit->toPlainText().split("\n"))
		system.push_back(l);
	paths["system"] = system;

	QJsonArray nuke;
	for (auto l : nuke_edit->toPlainText().split("\n"))
		nuke.push_back(l);
	paths["nuke"] = nuke;

	QJsonArray maya;
	for (auto l : maya_edit->toPlainText().split("\n"))
		maya.push_back(l);
	paths["maya"] = maya;

	QJsonArray houdini;
	for (auto l : houdini_edit->toPlainText().split("\n"))
		houdini.push_back(l);
	paths["houdini"] = houdini;

	QJsonArray cinema;
	for (auto l : cinema_edit->toPlainText().split("\n"))
		cinema.push_back(l);
	paths["cinema"] = cinema;

	QJsonArray fusion;
	for (auto l : fusion_edit->toPlainText().split("\n"))
		fusion.push_back(l);
	paths["fusion"] = fusion;

	QJsonArray ae;
	for (auto l : ae_edit->toPlainText().split("\n"))
		ae.push_back(l);
	paths["ae"] = ae;

	tcpClient(managerHost, 7000, jats({3, {{"preferences", {{"write", paths}}}}}));
}
