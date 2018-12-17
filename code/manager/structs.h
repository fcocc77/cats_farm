#ifndef STRUCT_H
#define STRUCT_H

struct task_struct
{
	QString name;
	QString status;
	int first_frame;
	int last_frame;
	QString server;
	QString time;
};

struct job_struct
{
	QString name;
	QString status;
	int priority;
	QStringList server;
	QStringList server_group;
	int instances;
	QString comment;
	QString submit_start;
	QString submit_finish;
	QString timer;
	QString timer2;
	QString total_render_time;
	QString estimated_time;
	float estimated_time_second;
	bool timer_last_active;
	QString software;
	QString project;
	QString system;
	QString extra;
	QString render;
	QStringList vetoed_servers;
	int progres;
	int old_p;
	int waiting_task;
	int tasks;
	int suspended_task;
	int failed_task;
	int active_task;
	int task_size;
	int first_frame;
	int last_frame;
	vector<task_struct *> task;
};

struct inst_struct
{
	int index;
	int status;
	bool reset;
	QString job_task;
};

struct server_struct
{
	QString name;
	QString status;
	QString host;
	QString system;
	int cpu;
	int cpu_cores;
	int ram;
	float ram_used;
	int ram_total;
	int temp;
	bool vbox;
	int response_time;
	QList<inst_struct *> instances;
	int max_instances;
	QString sshUser;
	QString sshPass;
	QString vmSoftware;
	QString schedule;
	bool schedule_state_0;
	bool schedule_state_1;
	QString log;
};

struct serverFromGroupStruct
{
	QString name;
	bool status;
};

struct group_struct
{
	QString name;
	bool status;
	int totaMachine;
	int activeMachine;
	QList<serverFromGroupStruct *> server;
};

struct makejob_struct
{
	QString job_name;
	QString server;
	QString server_group;
	int first_frame;
	int last_frame;
	int task_size;
	int priority;
	bool suspend;
	QString comment;
	QString software;
	QString project;
	QString extra;
	QString system;
	int instances;
	QString render;
};

#endif //STRUCT_H
