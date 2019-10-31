#include "../hpp/main_window.hpp"

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	shared = new shared_variables();

	global = new global_class(ui, this, shared);

	menu_bar = new main_menu_class(ui, global);
	jobs = new jobs_class(ui);
	servers = new servers_class(ui);
	tasks = new tasks_class(ui);
	groups = new groups_class(ui);
	servers = new servers_class(ui);
	settings = new settings_class(ui);
	update = new update_class(ui, shared);
	toolbar = new toolbar_class(ui, global);
	log = new log_class(ui);
	options = new options_class(ui);
}

monitor::~monitor()
{
	QString openMonitor = path + "/etc/openMonitor";
	fwrite(openMonitor, "0"); // escrebe que el monitor ya esta cerrado
}

/*

void monitor::closeEvent(QCloseEvent *event)
{
	if (shared->app_close)
		event->accept();
	else
	{
		event->ignore();
		this->hide();
	}
}

void monitor::init()
{

	// this->setCentralWidget(jobsList->widget);

	// main_menu();
	// tool_bar();
	// log_ui();
	// assamble();

	// qthread(&monitor::update, this);
}

void monitor::update()
{
	// si el archivo esta en 1 muestra el monitor
	static QString showMonitor = path + "/etc/showMonitor";
	if (fread(showMonitor).toInt())
	{
		this->show();
		fwrite(showMonitor, "0");
	} // ----------------------------
}

void monitor::assamble()
{
	this->setCentralWidget(jobsList->widget);
	this->addDockWidget(Qt::LeftDockWidgetArea, uiSubmit);
	this->addDockWidget(Qt::BottomDockWidgetArea, uiServerOptions);
	this->addDockWidget(Qt::BottomDockWidgetArea, serverList->dock);
	this->addDockWidget(Qt::BottomDockWidgetArea, groupList->dock);
	this->addDockWidget(Qt::RightDockWidgetArea, taskList->dock);
	this->addToolBar(toolBar);
	this->addDockWidget(Qt::LeftDockWidgetArea, uiJobOptions);
	this->addDockWidget(Qt::LeftDockWidgetArea, log_dock);
}



void monitor::tool_bar()
{
	QPushButton *tmpButton = new QPushButton();

	connect(tmpButton, &QPushButton::clicked, this, [this]() {
		_general->style_ui();
	});

	// toolBar
	toolBar->setObjectName("ToolBar");
	toolBar->addAction(jobActions->jobResumeAction);
	toolBar->addSeparator();
	toolBar->addAction(jobActions->jobSuspendAction);
	toolBar->addSeparator();
	toolBar->addAction(_general->preferencesAction);
	toolBar->addSeparator();
	toolBar->addAction(_general->panelSubmitAction);
	toolBar->addSeparator();
	toolBar->addWidget(tmpButton);
	toolBar->setIconSize(QSize(24, 24));
	toolBar->setMovable(0);
	toolBar->setWindowTitle("Tools Bar");
	//---------------------
}

void monitor::log_ui()
{
	// esto es para que el log tenga colores
	kgl::QCodeEditorDesign design(":/design.xml");
	QList<kgl::QSyntaxRule> rules = kgl::QSyntaxRules::loadFromFile(":/rule_cpp.xml", design);

	log_text->setDesign(design);
	log_text->setRules(rules);
	log_text->setKeywords({"printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset"});
	log_text->setReadOnly(true);
	//------------------------------------------

	QWidget *widget = new QWidget();
	QVBoxLayout *hbox = new QVBoxLayout();
	hbox->addWidget(log_text);
	widget->setLayout(hbox);

	log_text->setObjectName("Logs");
	log_dock->setObjectName("Logs");
	log_dock->setWidget(widget);
	log_dock->hide();
}


*/