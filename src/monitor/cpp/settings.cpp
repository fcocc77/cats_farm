#include "../hpp/settings.hpp"

settings_class::settings_class(
	Ui::MainWindow *_ui,
	shared_variables *_shared)
{
	ui = _ui;
	shared = _shared;

	property();
	connections();
}

settings_class::~settings_class()
{
}

void settings_class::property()
{
	ui->settings->hide();
	// ui->settings->setFloating(true);
}

void settings_class::connections()
{
	connect(ui->settings_cancel, &QPushButton::clicked, this, [this]() {
		ui->settings->hide();
	});
	connect(ui->settings_apply, &QPushButton::clicked, this, [this]() {
		this->ok();
	});
	connect(ui->settings_ok, &QPushButton::clicked, this, [this]() {
		this->ok();
		ui->settings->hide();
	});
}

void settings_class::update(QString host)
{
	shared->manager_host = host;
	path_read();
}

void settings_class::ok()
{
	path_write();

	// guarda host del manager
	QJsonObject manager = shared->settings["manager"].toObject();
	manager["ip"] = ui->settings_host->text();
	shared->settings["manager"] = manager;
	// ---------------------------

	// guarda lista de ips de QString a un json, y borra los espacios
	QString zones = ui->settings_zones->text();
	QJsonArray json_hosts;
	for (QString zone : zones.split(","))
		json_hosts.push_back(zone.replace(" ", ""));

	shared->settings["hosts"] = json_hosts;
	// -----------------------------
	jwrite(path + "/etc/settings.json", shared->settings);

	// agrega las ips al combobox de zonas
	ui->tool_zone->clear();
	for (QJsonValue ip : json_hosts)
		ui->tool_zone->addItem(ip.toString());
	// -------------------------
}

void settings_class::path_read()
{
	QJsonArray send = {"preferences", QJsonArray({"read", "none"})};
	QString recv = tcpClient(shared->manager_host, shared->manager_port, jats({3, send}));
	QJsonObject preferences = jofs(recv);

	auto array_to_string = [this](QJsonArray array) {
		QString ret;
		for (QJsonValue value : array)
			ret += value.toString() + "\n";
		return ret.left(ret.length() - 1); // borra la ultima linea
	};

	if (not preferences.empty())
	{
		QJsonObject paths = preferences["paths"].toObject();
		QString system, nuke, maya, houdini, cinema, natron, ae;

		system = array_to_string(paths["system"].toArray());
		nuke = array_to_string(paths["nuke"].toArray());
		houdini = array_to_string(paths["houdini"].toArray());
		cinema = array_to_string(paths["cinema"].toArray());
		natron = array_to_string(paths["natron"].toArray());
		maya = array_to_string(paths["maya"].toArray());
		ae = array_to_string(paths["ae"].toArray());

		ui->settings_paths->setPlainText(system);
		ui->settings_nuke->setPlainText(nuke);
		ui->settings_maya->setPlainText(maya);
		ui->settings_houdini->setPlainText(houdini);
		ui->settings_cinema->setPlainText(cinema);
		ui->settings_natron->setPlainText(natron);
		ui->settings_ae->setPlainText(ae);

		// setea los hosts guardados
		QString hosts;
		for (QJsonValue host : shared->settings["hosts"].toArray())
			hosts += host.toString() + ", ";
		hosts = hosts.left(hosts.length() - 2);

		ui->settings_zones->setText(hosts);
		// ------------------------------

		// setea el host del manager
		QString host = shared->settings["manager"].toObject()["ip"].toString();
		ui->settings_host->setText(host);
		// ------------------
	}
}

void settings_class::path_write()
{

	QJsonObject paths;

	QJsonArray system;
	for (auto l : ui->settings_paths->toPlainText().split("\n"))
		system.push_back(l);
	paths["system"] = system;

	QJsonArray nuke;
	for (auto l : ui->settings_nuke->toPlainText().split("\n"))
		nuke.push_back(l);
	paths["nuke"] = nuke;

	QJsonArray maya;
	for (auto l : ui->settings_maya->toPlainText().split("\n"))
		maya.push_back(l);
	paths["maya"] = maya;

	QJsonArray houdini;
	for (auto l : ui->settings_houdini->toPlainText().split("\n"))
		houdini.push_back(l);
	paths["houdini"] = houdini;

	QJsonArray cinema;
	for (auto l : ui->settings_cinema->toPlainText().split("\n"))
		cinema.push_back(l);
	paths["cinema"] = cinema;

	QJsonArray natron;
	for (auto l : ui->settings_natron->toPlainText().split("\n"))
		natron.push_back(l);
	paths["natron"] = natron;

	QJsonArray ae;
	for (auto l : ui->settings_ae->toPlainText().split("\n"))
		ae.push_back(l);

	paths["ae"] = ae;

	tcpClient(shared->manager_host, shared->manager_port, jats({3, {{"preferences", {{"write", paths}}}}}));
}
