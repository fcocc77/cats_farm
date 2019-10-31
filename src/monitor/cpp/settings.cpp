#include "../hpp/settings.hpp"

settings_class::settings_class(
	Ui::MainWindow *_ui)
{
	ui = _ui;
	manager_host = fread(path + "/etc/manager_host");

	property();
	connections();
	path_read();
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
		path_write();
	});
	connect(ui->settings_ok, &QPushButton::clicked, this, [this]() {
		QString _manager_ip = ui->settings_ip->text();
		fwrite(path + "/etc/manager_host", _manager_ip);

		path_write();
		ui->settings->hide();
	});
}

void settings_class::path_read()
{

	QJsonArray send = {"preferences", QJsonArray({"read", "none"})};
	QString recv = tcpClient(manager_host, 7000, jats({3, send}));
	QJsonObject preferences = jofs(recv);

	if (not preferences.empty())
	{
		QJsonObject paths = preferences["paths"].toObject();
		QString system, nuke, maya, houdini, cinema, fusion, ae;

		for (QJsonValue p : paths["system"].toArray())
			system += p.toString() + "\n";

		for (QJsonValue p : paths["nuke"].toArray())
			nuke += p.toString() + "\n";

		for (QJsonValue p : paths["houdini"].toArray())
			houdini += p.toString() + "\n";

		for (QJsonValue p : paths["cinema"].toArray())
			cinema += p.toString() + "\n";

		for (QJsonValue p : paths["fusion"].toArray())
			fusion += p.toString() + "\n";

		for (QJsonValue p : paths["maya"].toArray())
			maya += p.toString() + "\n";

		for (QJsonValue p : paths["ae"].toArray())
			ae += p.toString() + "\n";

		ui->settings_paths->setPlainText(system);
		ui->settings_nuke->setPlainText(nuke);
		ui->settings_maya->setPlainText(maya);
		ui->settings_houdini->setPlainText(houdini);
		ui->settings_cinema->setPlainText(cinema);
		ui->settings_fusion->setPlainText(fusion);
		ui->settings_ae->setPlainText(ae);
		ui->settings_ip->setText(manager_host);
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
		ae.push_back(l);
	paths["ae"] = ae;

	tcpClient(manager_host, 7000, jats({3, {{"preferences", {{"write", paths}}}}}));
}
