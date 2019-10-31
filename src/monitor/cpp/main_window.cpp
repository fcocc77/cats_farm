#include "../hpp/main_window.hpp"

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	// estilo de general
	QString style = fread(path + "/src/monitor/sass/main.css");
	this->setStyleSheet(style.toStdString().c_str());
	// ----------------------------

	shared = new shared_variables();
	log = new log_class(ui);

	jobs = new jobs_class(ui, shared, this, log);
	tasks = new tasks_class(ui, this);
	groups = new groups_class(ui, this, shared);
	servers = new servers_class(ui, this, shared, log);
	settings = new settings_class(ui);
	update = new update_class(ui, shared);

	global = new global_class(ui, this, shared);
	menu_bar = new main_menu_class(ui, global, jobs);
	toolbar = new toolbar_class(ui, global);
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



*/