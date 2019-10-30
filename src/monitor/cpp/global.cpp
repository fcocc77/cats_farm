#include "../hpp/global.hpp"

void global_class::init()
{
	// notifyIcon();
	// style_ui();
	connections();
}

void global_class::connections()
{

	// General Action
	connect(preferences_action, &QAction::triggered, this, [this]() {
		// _settings->show();
	});

	preferences_action->setIcon(QIcon(path + "/icons/setting.png"));
	preferences_action->setShortcut(QString("S"));


	/*
	connect(quitAction, &QAction::triggered, this, [this]() {
		shared->app_close = true;
		monitor->close();
	});

	connect(hideAction, &QAction::triggered, this, [this]() {
		if (monitor->isVisible())
			monitor->hide();
		else
			monitor->show();
	});
	hideAction->setShortcut(QString("Ctrl+Q"));

	connect(showAction, &QAction::triggered, this, [this]() {
		monitor->show();
	});

	connect(aboutAction, &QAction::triggered, this, &general::show_splash);

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
	menu->addAction(showAction);
	menu->addAction(quitAction);

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