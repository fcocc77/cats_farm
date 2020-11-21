#include <main_window.h>

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	setup_ui();
}

monitor::~monitor()
{
	QString openMonitor = path + "/etc/openMonitor";
	fwrite(openMonitor, "0"); // guarda un bool para ver si el monitor ya esta cerrado.
}

QWidget *monitor::add_title(QWidget *widget, QString title)
{
	// le agrega un titilo superior al 'widget'
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	container->setLayout(layout);

	QLabel *label = new QLabel(title);
	label->setObjectName("widget_title");
	layout->addWidget(label);
	layout->addWidget(widget);

	return container;
}

void monitor::setup_ui()
{
	shared = new shared_variables();
	shared->settings = jread(path + "/etc/settings.json");
	shared->manager_host = shared->settings["manager"].toObject()["ip"].toString();
	shared->manager_port = shared->settings["manager"].toObject()["port"].toInt();
	shared->server_port = shared->settings["server"].toObject()["port"].toInt();

	// Splitter
	QSplitter *splitter_main = new QSplitter(this);
	splitter_main->setOrientation(Qt::Vertical);
	this->setCentralWidget(splitter_main);

	QSplitter *splitter_top = new QSplitter(this);
	QSplitter *splitter_bottom = new QSplitter(this);

	//

	//

	// propiedades
	log = new log_class();
	options = new options_class();
	settings = new settings_class(shared);
	//

	// Widget de propiedades
	properties = new properties_class(log, options, settings);
	QWidget *properties_parent = add_title(properties, "Properties");
	properties_parent->hide();
	properties_parent->setMaximumWidth(500);
	splitter_top->addWidget(properties_parent);
	//

	//

	// Servers
	servers = new servers_class(this, shared, log);
	QWidget *servers_parents = add_title(servers, "Servers");
	splitter_bottom->addWidget(servers_parents);
	//

	//

	// Groups
	groups = new groups_class(this, shared, servers);
	QWidget *groups_parent = add_title(groups, "Groups");
	splitter_bottom->addWidget(groups_parent);
	//

	//

	// Jobs
	jobs = new jobs_class(shared, this, log, servers, options, groups, properties);
	splitter_top->addWidget(add_title(jobs, "Jobs"));
	//

	//

	// Task
	tasks = new tasks_class(this, shared, jobs);
	QWidget *tasks_parent = add_title(tasks->tree, "Tasks");
	splitter_top->addWidget(tasks_parent);
	//

	//

	//

	update = new update_class(shared, groups, jobs, servers, tasks->tree, settings);
	global = new global_class(this, shared, properties);

	main_menu = new main_menu_class(global, jobs, servers, groups, tasks);
	this->setMenuBar(main_menu);

	toolbar = new toolbar_class(this, global, jobs, tasks, servers, groups, log, update, shared, settings, properties);
	toolbar->setMaximumHeight(40);
	//

	//

	// Sumar widget al splitter principal
	splitter_main->addWidget(toolbar);
	splitter_main->addWidget(splitter_top);
	splitter_main->addWidget(splitter_bottom);

	// Tamanio de Splitters
	splitter_main->setSizes({10, 100, 10});
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