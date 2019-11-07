#include "../hpp/main_window.hpp"

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	shared = new shared_variables();
	shared->settings = jread(path + "/etc/settings.json");
	shared->manager_host = shared->settings["manager"].toObject()["ip"].toString();
	shared->manager_port = shared->settings["manager"].toObject()["port"].toInt();
	shared->server_port = shared->settings["server"].toObject()["port"].toInt();

	log = new log_class(ui);

	jobs = new jobs_class(ui, shared, this, log);
	tasks = new tasks_class(ui, this, shared);
	groups = new groups_class(ui, this, shared);
	servers = new servers_class(ui, this, shared, log);
	settings = new settings_class(ui, shared);
	update = new update_class(ui, shared, groups, settings);

	global = new global_class(ui, this, shared);
	main_menu = new main_menu_class(ui, global, jobs, servers, groups, tasks);
	toolbar = new toolbar_class(ui, global, jobs, update, shared);
	options = new options_class(ui);
}

monitor::~monitor()
{
	QString openMonitor = path + "/etc/openMonitor";
	fwrite(openMonitor, "0"); // guarda un bool para ver si el monitor ya esta cerrado.
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