#include "general.h"

void general::init()
{
	notifyIcon();
	style_ui();
	actions();
}

void general::actions()
{

	// General Action
	connect(preferencesAction, &QAction::triggered, this, [this]() {
		_settings->show();
	});

	preferencesAction->setIcon(QIcon(path + "/icons/setting.png"));
	preferencesAction->setShortcut(QString("S"));

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
}

void general::style_ui()
{

	monitor->setWindowTitle("CatsFarm Monitor");

	QIcon icon(path + "/icons/monitor.png");
	monitor->setWindowIcon(icon);

	QString style = fread(path + "/theme/style.css");

	monitor->setStyleSheet(style.toStdString().c_str());
}

void general::show_splash()
{
}

void general::notifyIcon()
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

	threading([this]() {
		int itemCount = jobsList->topLevelItemCount();

		for (int i = 0; i < itemCount; i++)
		{

			QTreeWidgetItem *item = jobsList->topLevelItem(i);
			QString status = item->text(4);
			QString name = item->text(0);

			QString notifyJob = path + "/log/trayIcon/" + name;

			if (status == "Queue" or status == "Rendering...")
				if (os::isfile(notifyJob))
					os::remove(notifyJob);

			if (status == "Failed" or status == "Completed")
			{
				if (not os::isfile(notifyJob))
				{
					fwrite(notifyJob, "0");

					if (status == "Failed")
					{
						notify->showMessage("CatsFarm", name + "  has failed");
						QSound(path + "/sound/angry_cat.wav", this).play();
					}

					if (status == "Completed")
					{
						notify->showMessage("CatsFarm", name + "  has finished");
						QSound(path + "/sound/meaw_cat.wav", this).play();
					}
				}
			}
		}
	});
}

void general::fileOpen()
{
}

void general::fileOutputOpen()
{
}
