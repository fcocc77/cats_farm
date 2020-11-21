#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream>
#include <vector>
using namespace std;
#include <ctime>	 // time_t
#include <algorithm> //sort
#include <unistd.h>	 // sleep usleep
#include <thread>	 // thread
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QTime>

#include <structs.h>
#include <util.h>
#include <os.h>
#include "../utils/video.h"
#include "../utils/tcp.h"
#include "../utils/threading.h"

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
	void reset_all_servers();
	void kill_tasks(job_struct *job, bool _delete);
	void render_job();
	void render_task(server_struct *server, inst_struct *instance, job_struct *job);
	QString send_to_monitor_thread();
	QString send_to_logger();
	QJsonObject struct_to_json();
	void json_to_struct(QJsonObject info);
	void reactive_all();
	QString recieve_monitor_thread(QJsonArray recv);
	QString server_action(QJsonArray pks);
	void group_action(QJsonArray pks);
	void task_action(QJsonArray pks);
	void group_create(QJsonArray pks);
	QString preferences_action(QJsonArray pks);
	void server_set_state(server_struct *server, bool state);
	QString server_tcp(QString recv);

	// jobs
	void job_delete(QString job_name);
	void job_action(QJsonArray pks);
	QString job_options(QJsonArray pks);
	QString job_log_action(QString pks);

	// funciones para videovina
	void sample_render(QString video, int frame, int index);
	void samples_export(QString video, QJsonArray ranges);
	void videovina(QJsonArray recv);
	void send_to_render(job_struct *job);
	void post_render(QJsonObject extra, int last_frame, QString job_name);
	// ---------------------

	void ae_completed(job_struct *job);
	void nuke_completed(job_struct *job, QString src_path, QString dst_path);
	void ntp_completed(job_struct *job);
	void natron_completed(job_struct *job, QString src_path, QString dst_path);

	QString update_server_thread(QJsonArray recv);
	void update_server();
	void update_group();
	void update_jobs();
	void container_save();
	void update_all();
	job_struct *get_job(QString name);
	server_struct *get_server(QString name);
	group_struct *get_group(QString name);
	task_struct *get_task(vector<task_struct *> tasks, QString name);

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