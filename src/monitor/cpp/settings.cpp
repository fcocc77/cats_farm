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

	QString hosts = ui->settings_ip->text();
	QStringList ips = hosts.split(",");
	// guarda lista de ips de string a un array json
	QJsonArray json_hosts;
	for (QString host : ips)
		json_hosts.push_back(host);

	shared->settings["hosts"] = json_hosts;
	// -----------------------------
	jwrite(path + "/etc/settings.json", shared->settings);

	// agrega las ips al combobox de zonas
	ui->tool_zone->clear();
	for (QString ip : ips)
		ui->tool_zone->addItem(ip);
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
		QString system, nuke, maya, houdini, cinema, fusion, ae;

		system = array_to_string(paths["system"].toArray());
		nuke = array_to_string(paths["nuke"].toArray());
		houdini = array_to_string(paths["houdini"].toArray());
		cinema = array_to_string(paths["cinema"].toArray());
		fusion = array_to_string(paths["fusion"].toArray());
		maya = array_to_string(paths["maya"].toArray());
		ae = array_to_string(paths["ae"].toArray());

		ui->settings_paths->setPlainText(system);
		ui->settings_nuke->setPlainText(nuke);
		ui->settings_maya->setPlainText(maya);
		ui->settings_houdini->setPlainText(houdini);
		ui->settings_cinema->setPlainText(cinema);
		ui->settings_fusion->setPlainText(fusion);
		ui->settings_ae->setPlainText(ae);

		// setea los hosts guardados
		QString hosts;
		for (QJsonValue host : shared->settings["hosts"].toArray())
			hosts += host.toString() + ", ";
		hosts.left(hosts.length() - 2);

		ui->settings_ip->setText(hosts);
		// ------------------------------
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

	QJsonArray fusion;
	for (auto l : ui->settings_fusion->toPlainText().split("\n"))
		fusion.push_back(l);
	paths["fusion"] = fusion;

	QJsonArray ae;
	for (auto l : ui->settings_ae->toPlainText().split("\n"))
	{
		ae.push_back(l);
	}
	paths["ae"] = ae;
	print(QString::number(ae.size()));

	tcpClient(shared->manager_host, shared->manager_port, jats({3, {{"preferences", {{"write", paths}}}}}));
}
