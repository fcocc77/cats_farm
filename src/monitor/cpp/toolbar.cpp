#include <toolbar.hpp>

toolbar_class::toolbar_class(
	global_class *_global,
	jobs_class *_jobs,
	update_class *_update,
	shared_variables *_shared,
	QDockWidget *_settings_dock)
{
	global = _global;
	jobs = _jobs;
	update = _update;
	shared = _shared;
	settings_dock = _settings_dock;
	setup_ui();
	load_zones();
	connections();
}

toolbar_class::~toolbar_class()
{
}

void toolbar_class::setup_ui()
{
	QWidget *widget = new QWidget();
	widget->setObjectName("barwidget");
	widget->setContentsMargins(0,0,0,0);
	QHBoxLayout *main_layout = new QHBoxLayout();
	widget->setLayout(main_layout);

	this->addWidget(widget);
	this->setMovable(0);
	this->setIconSize(QSize(30, 30));
	this->setObjectName("toolbar");

	resume = new QPushButton("RESUME JOB");
	resume->setObjectName("resume");
	main_layout->addWidget(resume);

	suspend = new QPushButton("SUSPEND JOB");
	suspend->setObjectName("suspend");
	main_layout->addWidget(suspend);

	settings = new QPushButton("SETTINGS");
	settings->setObjectName("settings");
	main_layout->addWidget(settings);

	QLabel *zone = new QLabel("ZONE:");
	zone->setObjectName("zone");
	main_layout->addWidget(zone);

	shared->zone_box = new QComboBox();
	shared->zone_box->setObjectName("zone_box");
	main_layout->addWidget(shared->zone_box);

	shared->conection = new QLabel();
	shared->conection->setObjectName("conection");
	main_layout->addWidget(shared->conection);

	QLabel *jobs_label = new QLabel("QUEUE JOBS:");
	jobs_label->setObjectName("jobs_label");
	main_layout->addWidget(jobs_label);

	shared->jobs_count = new QLabel("0 jobs.");
	shared->jobs_count->setObjectName("jobs_count");
	main_layout->addWidget(shared->jobs_count);
}

void toolbar_class::connections()
{
	connect(settings, &QPushButton::clicked, this, [this]() {
		settings_dock->show();
	});

	connect(suspend, &QPushButton::clicked, this, [this]() {
		jobs->job_suspend_action->triggered();
	});

	connect(resume, &QPushButton::clicked, this, [this]() {
		jobs->job_resume_action->triggered();
	});

	connect(shared->zone_box, &QComboBox::currentTextChanged, update, &update_class::update);
}

void toolbar_class::load_zones()
{
	QJsonArray zones = shared->settings["hosts"].toArray();
	QString current_manager = shared->settings["current_manager"].toString();

	// agrega todas las zonas guardadas al combobox
	for (QJsonValue value : zones)
	{
		QString zone = value.toString();
		shared->zone_box->addItem(zone);
	}
	// ---------------------------

	// check zona actual en el combobox
	shared->zone_box->setCurrentText(current_manager);
	// ----------------------
	// establece por defecto la ultima zona guardada
	update->update(current_manager);
	// ----------------------
}
