#include "../hpp/main_window.hpp"

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	shared = new shared_variables();

	menu_bar = new menu_bar_class(ui);
	jobs = new jobs_class(ui);
	servers = new servers_class(ui);
	tasks = new tasks_class(ui);
	groups = new groups_class(ui);
	servers = new servers_class(ui);
	update = new update_class(ui, shared);
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

void monitor::main_menu()
{
	// Main Menu-------------------------------------
	QMenuBar *mainMenu = menuBar();
	QMenu *fileMenu = new QMenu("&File", this);
	mainMenu->addMenu(fileMenu);
	fileMenu->addAction(_general->hideAction);
	fileMenu->addAction(_general->quitAction);

	QMenu *editMenu = new QMenu("&Edit", this);
	mainMenu->addMenu(editMenu);
	editMenu->addAction(_general->preferencesAction);
	editMenu->addAction(_general->panelSubmitAction);
	editMenu->addAction(_general->hidePanelsAction);

	QMenu *serverMenu = new QMenu("&Servers", this);
	mainMenu->addMenu(serverMenu);
	serverMenu->addAction(serverActions->serverInactiveAction);
	serverMenu->addAction(serverActions->serverReactiveAction);
	serverMenu->addAction(serverActions->deleteAction);
	serverMenu->addSeparator();
	serverMenu->addAction(serverActions->serverMaxInstancesAction);
	serverMenu->addSeparator();
	serverMenu->addAction(serverActions->serverShowLog);
	serverMenu->addAction(serverActions->serverFreeramAction);
	serverMenu->addAction(serverActions->serverSshAction);

	QMenu *groupMenu = new QMenu("&Groups", this);
	mainMenu->addMenu(groupMenu);
	groupMenu->addAction(groupActions->groupCreateAction);
	groupMenu->addAction(groupActions->deleteAction);
	groupMenu->addSeparator();
	groupMenu->addAction(groupActions->groupAddmachineAction);

	QMenu *jobMenu = new QMenu("&Jobs", this);
	mainMenu->addMenu(jobMenu);
	jobMenu->addAction(jobActions->jobResumeAction);
	jobMenu->addAction(jobActions->jobSuspendAction);
	jobMenu->addAction(jobActions->deleteAction);
	jobMenu->addAction(jobActions->jobRestartAction);
	jobMenu->addSeparator();
	jobMenu->addAction(jobActions->jobShowRenderFolderAction);
	jobMenu->addSeparator();
	jobMenu->addAction(jobActions->jobModifyAction);
	jobMenu->addAction(jobActions->jobLogAction);

	QMenu *taskMenu = new QMenu("&Tasks", this);
	mainMenu->addMenu(taskMenu);
	taskMenu->addAction(taskActions->taskSuspendAction);
	taskMenu->addSeparator();
	taskMenu->addAction(taskActions->taskRestartAction);

	QMenu *helpMenu = new QMenu("&Help", this);
	mainMenu->addMenu(helpMenu);
	helpMenu->addAction(_general->aboutAction);

	//--------------------------------------------------
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