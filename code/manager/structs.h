#ifndef STRUCT_H
#define STRUCT_H


struct task_struct{
	string name;
	string status;
	int first_frame;
	int last_frame;
	string server;
	string time;
};

struct job_struct{
	string name;
	string status;
	int priority;
	vector <string> server;
	vector <string> server_group;
	int instances;
	string comment;
	string submit_start;
	string submit_finish;
	string timer;
	string timer2;
	string total_render_time;
	string estimated_time;
	float estimated_time_second;
	bool timer_last_active;	
	string software;
	string project;
	string system;
	string extra;
	string render;
	vector <string> vetoed_servers;
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
	vector <task_struct*> task;
};

struct inst_struct{
	int index;
	int status;
	bool reset;
	string job_task;
};

struct server_struct{
	string name;
	string status;
	string host;
	string system;
	int cpu;
	int cpu_cores;
	int ram;
	float ram_used;
	int ram_total;
	int temp;
	bool vbox;
	int response_time;
	vector <inst_struct*> instances;
	int max_instances;
	string sshUser;
	string sshPass;
	string vmSoftware;
	string schedule;
	bool schedule_state_0;
	bool schedule_state_1;
	string log;
};


struct serverFromGroupStruct{
	string name;
	bool status;
};

struct group_struct {
	string name;
	bool status;
	int totaMachine;
	int activeMachine;
	vector <serverFromGroupStruct*> server;
};

struct makejob_struct{
		string job_name; 
		string server; 
		string server_group; 
		int first_frame; 
		int last_frame; 
		int task_size; 
		int priority; 
		bool suspend; 
		string comment; 
		string software; 
		string project; 
		string extra; 
		string system; 
		int instances;
		string render;
};



#endif //STRUCT_H
