#include "../hpp/main_window.hpp"

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

	QSplitter *splitter_main = new QSplitter(this);
	splitter_main->setOrientation(Qt::Vertical);
	this->setCentralWidget(splitter_main);

	QSplitter *splitter_top = new QSplitter(this);
	QSplitter *splitter_bottom = new QSplitter(this);

	splitter_main->addWidget(splitter_top);
	splitter_main->addWidget(splitter_bottom);
	splitter_main->setSizes({70, 30});

	log = new log_class();
	log->hide();

	options = new options_class();

	// widget de propiedades
	QWidget *properties = new QWidget();
	QVBoxLayout *properties_layout = new QVBoxLayout();
	properties->setLayout(properties_layout);
	properties_layout->addWidget(log);
	properties_layout->addWidget(options);
	splitter_top->addWidget(properties);

	servers = new servers_class(this, shared, log);
	splitter_bottom->addWidget(servers);

	groups = new groups_class(this, shared, servers);
	splitter_bottom->addWidget(groups);

	jobs = new jobs_class(shared, this, log, servers, options, groups);
	// QWidget *jobs_container = new QWidget;
	// QVBoxLayout *jobs_container_layout = new QVBoxLayout();
	// jobs_container->setLayout(jobs_container_layout);
	// QLabel *jobs_label = new QLabel("Jobs");
	// jobs_label->setObjectName("jobs_title");
	// jobs_container_layout->addWidget(jobs_label);
	// // jobs_container_layout->addWidget(jobs);
	splitter_top->addWidget(jobs);

	tasks = new tasks_class(this, shared, jobs);
	splitter_top->addWidget(tasks->tree);

	settings = new settings_class(shared);
	QDockWidget *settings_dock = new QDockWidget("Settings");
	settings_dock->hide();
	settings_dock->setWidget(settings);

	update = new update_class(shared, groups, jobs, servers, tasks->tree, settings);
	global = new global_class(this, shared, settings_dock, options, log, properties);

	main_menu = new main_menu_class(global, jobs, servers, groups, tasks);
	this->setMenuBar(main_menu);

	toolbar = new toolbar_class(global, jobs, update, shared, settings_dock);
	this->addToolBar(toolbar);

	splitter_top->setSizes({120, 300, 120});
	splitter_bottom->setSizes({300, 120});
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