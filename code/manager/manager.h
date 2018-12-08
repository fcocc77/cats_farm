#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
using namespace std;
#include <ctime>	 // time_t
#include <algorithm> //sort
#include <unistd.h>  // sleep usleep
#include <thread>	// thread
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "structs.h"
#include "../utils/video.h"
#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/os.h"
#include "../utils/threading.h"

class manager : public QObject
{
  public:
	QList<job_struct *> jobs;
	QList<server_struct *> servers;
	QList<group_struct *> groups;
	QJsonObject preferences = jread("../../etc/preferences.json");
	bool reset_render;

	void init();
	QString make_job(QJsonArray recv);
	QList<task_struct *> make_task(int first_frame, int last_frame, int task_size);
	void resetAllServer();
	void kill_tasks(job_struct *job, bool _delete);
	void render_job();
	void render_task(server_struct *server, inst_struct *instance, job_struct *job);
	QString sendToMonitor_thread();
	QJsonObject struct_to_json();
	void json_to_struct(QJsonObject info);
	void reactive_all();
	QString recieve_monitor_thread(QJsonArray recv);
	void jobAction(QJsonArray pks);
	QString jobOptions(QJsonArray pks);
	QString serverAction(QJsonArray pks);
	QString serverOptions(QJsonArray pks);
	void groupAction(QJsonArray pks);
	void taskAction(QJsonArray pks);
	void groupCreate(QJsonArray pks);
	QString preferencesAction(QJsonArray pks);
	QString jobLogAction(QString pks);
	void serverSetState(server_struct *server, bool state);
	QString server_tcp(QString recv);

	QString update_server_thread(QJsonArray recv);
	void update_server();
	bool iTime(QString schedule);
	void update_group();
	void update_jobs();
	void container_save();
	void update_all();

	// encuentra un index por el nombre de un vector de estructuras
	template <typename T> // para que el retorno sea diferente al primer argumento
	auto *find_struct(T lista, QString name)
	{
		for (auto s : lista)
		{
			if (s->name == name)
			{
				return s;
			}
		}
		return lista[0];
	}
	//-----------------------------------------------------------------

	template <typename T>
	bool is_struct(T lista, QString name)
	{
		for (auto s : lista)
		{
			if (s->name == name)
			{
				return true;
			}
		}
		return false;
	}

	void erase_by_name(auto &lista, QString name)
	{
		int i = 0;
		for (auto s : lista)
		{
			if (s->name == name)
			{
				break;
			}
			i++;
		}

		lista.erase(lista.begin() + i, lista.begin() + i + 1);
	}
};

#endif //MANAGER_H