#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream>
#include <vector>
using namespace std;
#include <ctime>	 // time_t
#include <algorithm> //sort
#include <unistd.h>  // sleep usleep
#include <thread>	// thread
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QTime>

#include "structs.hpp"
#include "../utils/video.hpp"
#include "../utils/util.hpp"
#include "../utils/tcp.hpp"
#include "../utils/os.hpp"
#include "../utils/threading.hpp"

class manager : public QObject
{
public:
	vector<job_struct *> jobs;
	vector<server_struct *> servers;
	vector<group_struct *> groups;
	QJsonObject preferences = jread(path + "/etc/preferences.json");
	bool reset_render;
	QMutex mutex;
	QJsonObject settings;
	int server_port;

	// rutas generales para videovina
	QJsonObject env_videovina = jread(path + "/etc/videovina.json");
	QString assets = env_videovina["assets"].toString();
	QString as3 = env_videovina["s3"].toString();
	QString vinarender = env_videovina["vinarender"].toString();
	QString vv_local_folder = env_videovina["local"].toString();
	// -------------------------------

	manager();

	QString make_job(QJsonArray recv);
	QString pivot_to_server(QJsonArray recv);
	vector<task_struct *> make_task(int first_frame, int last_frame, int task_size);
	void resetAllServer();
	void kill_tasks(job_struct *job, bool _delete);
	void render_job();
	void render_task(server_struct *server, inst_struct *instance, job_struct *job);
	QString sendToMonitor_thread();
	QString sendToLogger();
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
	QString videovina(QJsonArray recv);

	QString update_server_thread(QJsonArray recv);
	void update_server();
	bool iTime(QString schedule);
	void update_group();
	void update_jobs();
	void container_save();
	void update_all();
	job_struct *findJob(QString name);
	server_struct *findServer(QString name);
	group_struct *findGroup(QString name);
	task_struct *findTask(vector<task_struct *> tasks, QString name);

	template <typename T>
	bool is_struct(T lista, QString name)
	{
		for (auto s : lista)
			if (s->name == name)
				return true;

		return false;
	}

	template <typename T>
	void erase_by_name(T &lista, QString name)
	{
		int i = 0;
		for (auto s : lista)
		{
			if (s->name == name)
				break;

			i++;
		}

		lista.erase(lista.begin() + i, lista.begin() + i + 1);
	}
};

#endif //MANAGER_HPP