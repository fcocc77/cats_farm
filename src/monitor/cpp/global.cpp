#include "../hpp/global.hpp"

global_class::global_class(
	Ui::MainWindow *_ui,
	QMainWindow *_monitor,
	shared_variables *_shared)
{

	// _settings = new settings(_monitor);
	ui = _ui;
	monitor = _monitor;
	shared = _shared;
	// uiSubmit = _monitor->uiSubmit;
	// uiJobOptions = _monitor->uiJobOptions;
	// uiServerOptions = _monitor->uiServerOptions;
	// log_dock = _monitor->log_dock;
	// jobsList = _monitor->jobsList;
	// shared = _monitor->shared;

	// notifyIcon();
	// style_ui();
	connections();
}

void global_class::connections()
{
	connect(preferences_action, &QAction::triggered, this, [this]() {
		ui->settings->show();
	});

	preferences_action->setIcon(QIcon(path + "/icons/setting.png"));
	preferences_action->setShortcut(QString("S"));

	connect(quit_action, &QAction::triggered, this, [this]() {
		shared->app_close = true;
		monitor->close();
	});

	connect(hide_action, &QAction::triggered, this, [this]() {
		if (monitor->isVisible())
			monitor->hide();
		else
			monitor->show();
	});
	hide_action->setShortcut(QString("Ctrl+Q"));

	connect(show_action, &QAction::triggered, this, [this]() {
		monitor->show();
	});

	/*

	connect(panelSubmitAction, &QAction::triggered, this, [this]() {
		uiSubmit->savePanel = true;
		uiSubmit->submitUpdateBox();
        uiSubmit->submitPanelOpen();
		uiSubmit->show(); });

	panelSubmitAction->setIcon(QIcon(path + "/icons/submit.png"));

	connect(hidePanelsAction, &QAction::triggered, this, [this]() {
		uiSubmit->hide();
		uiJobOptions->hide();
		uiServerOptions->hide();
		log_dock->hide();
	});
	hidePanelsAction->setShortcut(QString("Esc"));
	//---------------------------------------------------

	*/
}
/*
void global_class::style_ui()
{

	monitor->setWindowTitle("VinaRender Monitor");

	QIcon icon(path + "/icons/monitor.png");
	monitor->setWindowIcon(icon);

	QString style = fread(path + "/src/monitor/sass/main.css");

	// monitor->setStyleSheet(style.toStdString().c_str());
}

void global_class::show_splash()
{
}

void global_class::notifyIcon()
{
	notify = new QSystemTrayIcon(QIcon(path + "/icons/monitor.png"), monitor);

	connect(notify, &QSystemTrayIcon::messageClicked, this, [this]() {
		monitor->show();
	});

	connect(notify, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::Trigger)
		{
			// al clickear el icono muestra o oculta la ventana principal
			if (monitor->isVisible())
				monitor->hide();
			else
				monitor->show();
			//------------------------------------------------
		}
	});
	notify->show();

	QMenu *menu = new QMenu(monitor);
	menu->addAction(show_action);
	menu->addAction(quit_action);

	notify->setContextMenu(menu);

	// Importa lista de jobs completados
	for (QJsonValue job : jafs(fread(path + "/log/trayIcon")))
		completedJobs.push_back(job.toString());
	// ----------------------------------------------

	threading([this]() {
		while (1)
		{
			QStringList inListJobs;
			for (int i = 0; i < jobsList->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *item = jobsList->topLevelItem(i);
				QString status = item->text(4);
				QString name = item->text(0);
				inListJobs.push_back(name);

				if (status == "Queue" or status == "Rendering...")
					completedJobs.removeOne(name);

				if (status == "Failed" or status == "Completed")
				{
					if (not completedJobs.contains(name))
					{
						completedJobs.push_back(name);

						// Guarda lista de jobs completados
						fwrite(path + "/log/trayIcon", jats(QJsonArray::fromStringList(completedJobs)));
						//---------------------------------

						if (status == "Failed")
						{
							notify->showMessage("VinaRender", name + "  has failed");
							QSound(path + "/sound/angry_cat.wav", this).play();
						}

						if (status == "Completed")
						{
							notify->showMessage("VinaRender", name + "  has finished");
							QSound(path + "/sound/meaw_cat.wav", this).play();
						}
					}
				}
			}

			// Borra los job que ya no estan en la jobList
			for (QString job : completedJobs)
			{
				if (not inListJobs.contains(job))
				{
					completedJobs.removeOne(job);
					// Guarda lista de jobs completados
					fwrite(path + "/log/trayIcon", jats(QJsonArray::fromStringList(completedJobs)));
					//---------------------------------
				}
			} //-----------------------------
			sleep(1);
		}
	});
}
*/