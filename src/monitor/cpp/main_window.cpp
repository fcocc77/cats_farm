#include "../hpp/main_window.hpp"

monitor::monitor(QWidget *parent) : QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	shared = new shared_variables();
	log = new log_class(ui);

	jobs = new jobs_class(ui, shared, this, log);
	tasks = new tasks_class(ui, this);
	groups = new groups_class(ui, this, shared);
	servers = new servers_class(ui, this, shared, log);
	settings = new settings_class(ui);
	update = new update_class(ui, shared, groups);

	global = new global_class(ui, this, shared);
	main_menu = new main_menu_class(ui, global, jobs, servers, groups, tasks);
	toolbar = new toolbar_class(ui, global, jobs);
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