#include "manager.hpp"

void manager::update_all()
{
	while (1)
	{
		mutex.lock();
		update_jobs();
		container_save();
		update_server();
		update_group();
		mutex.unlock();

		sleep(1);
	}
}

void manager::container_save()
{
	static int sec;
	sec++;

	// guarda informacion a json cada 10 seg
	if (sec > 10)
	{
		jwrite(path + "/etc/info.json", struct_to_json());
		sec = 0;
	}
}
