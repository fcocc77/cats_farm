// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef HARDWARE_MONITOR_H
#define HARDWARE_MONITOR_H

#include <QList>
#include <QString>

namespace hm
{

int get_cpu_used();
int get_iowait_cpu_used();
int get_process_cpu_used(int pid);
int get_ram_percent(bool cached_percent = false);
float get_ram_total();
float get_ram_used();
int get_cpu_temp();
int get_cpu_cores();
QList<long> get_cpu_stat();
QList<float> get_ram_from_windows();

}; // namespace hm

#endif // HARDWARE_MONITOR_H
