#include <main_window.hpp>

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	setup_ui();
}

monitor::~monitor()
{
	QString openMonitor = path + "/etc/openMonitor";
	fwrite(openMonitor, "0"); // guarda un bool para ver si el monitor ya esta cerrado.
}

void monitor::setup_ui()
{
	shared = new shared_variables();
	shared->settings = jread(path + "/etc/settings.json");
	shared->manager_host = shared->settings["manager"].toObject()["ip"].toString();
	shared->manager_port = shared->settings["manager"].toObject()["port"].toInt();
	shared->server_port = shared->settings["server"].toObject()["port"].toInt();

	log = new log_class();
	QDockWidget *log_dock = new QDockWidget("Log");
	log_dock->hide();
	log_dock->setWidget(log);
	this->addDockWidget(Qt::LeftDockWidgetArea, log_dock);

	options = new options_class();
	QDockWidget *options_dock = new QDockWidget("Options");
	options_dock->hide();
	options_dock->setWidget(options);
	this->addDockWidget(Qt::LeftDockWidgetArea, options_dock);

	servers = new servers_class(this, shared, log);
	QDockWidget *servers_dock = new QDockWidget("Servers");
	servers_dock->setWidget(servers);
	this->addDockWidget(Qt::BottomDockWidgetArea, servers_dock);

	groups = new groups_class(this, shared, servers);
	QDockWidget *group_dock = new QDockWidget("Groups");
	group_dock->setWidget(groups);
	this->addDockWidget(Qt::BottomDockWidgetArea, group_dock);

	jobs = new jobs_class(shared, this, log, servers, options, groups);
	QWidget *jobs_container = new QWidget;
	QVBoxLayout *jobs_container_layout = new QVBoxLayout();
	jobs_container->setLayout(jobs_container_layout);
	QLabel *jobs_label = new QLabel("Jobs");
	jobs_label->setObjectName("jobs_title");
	jobs_container_layout->addWidget(jobs_label);
	jobs_container_layout->addWidget(jobs);
	this->setCentralWidget(jobs_container);

	tasks = new tasks_class(this, shared, jobs);
	QDockWidget *tasks_dock = new QDockWidget("Tasks");
	tasks_dock->setWidget(tasks->tree);
	this->addDockWidget(Qt::RightDockWidgetArea, tasks_dock);

	settings = new settings_class(shared);
	QDockWidget *settings_dock = new QDockWidget("Settings");
	settings_dock->hide();
	settings_dock->setWidget(settings);
	this->addDockWidget(Qt::LeftDockWidgetArea, settings_dock);

	update = new update_class(shared, groups, jobs, servers, tasks->tree, settings);
	global = new global_class(this, shared, settings_dock, options_dock, log_dock);

	main_menu = new main_menu_class(global, jobs, servers, groups, tasks);
	this->setMenuBar(main_menu);

	toolbar = new toolbar_class(global, jobs, update, shared, settings_dock);
	this->addToolBar(toolbar);
}

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