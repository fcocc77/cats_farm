#include <hardware_monitor.h>
#include "../global/global.h"
#include <os.h>

namespace hm
{

QList<long> get_cpu_stat()
{
    QList<long> cpu;

    QString s = os::sh("cat /proc/stat");
    QStringList stat = s.split(" ");

    for (int i = 2; i < 10; i++)
        cpu.push_back(stat[i].toLong());

    return cpu;
}

int get_cpu_used()
{
    static int usage;
    QString result;

    if (_linux)
    {
        static QList<long> current;
        QList<long> prev;
        long prev_idle, idle, prev_not_idle, not_idle, prev_total, total;
        float totald, idled;

        if (current.empty())
        {
            current = get_cpu_stat();
            prev = get_cpu_stat();
        }
        else
        {
            prev = current;
            current = get_cpu_stat();
        }

        prev_idle = prev[3] + prev[4];
        idle = current[3] + current[4];

        // 0:user, 1:nice, 2:system, 3:idle, 4:iowait, 5:irq, 6:softirq,
        // 7:steal, 8:guest
        prev_not_idle =
            prev[0] + prev[1] + prev[2] + prev[5] + prev[6] + prev[7];
        not_idle = current[0] + current[1] + current[2] + current[5] +
                   current[6] + current[7];

        prev_total = prev_idle + prev_not_idle;
        total = idle + not_idle;

        totald = total - prev_total;
        idled = idle - prev_idle;

        usage = round(((totald - idled) / totald) * 100.0);
    }

    return usage;
}

int get_iowait_cpu_used()
{

    static int usage;
    QString result;

    if (_linux)
    {
        static QList<long> current;
        QList<long> prev;
        long prev_idle, idle, prev_not_idle, not_idle, prev_total, total;
        float totald, idled;

        if (current.empty())
        {
            current = get_cpu_stat();
            prev = get_cpu_stat();
        }
        else
        {
            prev = current;
            current = get_cpu_stat();
        }

        prev_idle = prev[3];
        idle = current[3];

        // 0:user, 1:nice, 2:system, 3:idle, 4:iowait, 5:irq, 6:softirq,
        // 7:steal, 8:guest
        prev_not_idle =
            prev[0] + prev[1] + prev[2] + prev[4] + prev[5] + prev[6] + prev[7];
        not_idle = current[0] + current[1] + current[2] + current[4] +
                   current[5] + current[6] + current[7];

        prev_total = prev_idle + prev_not_idle;
        total = idle + not_idle;

        totald = total - prev_total;
        idled = idle - prev_idle;

        usage = round(((totald - idled) / totald) * 100.0);
    }

    return usage;
}

int get_process_cpu_used(int pid)
{
    // obtiene el uso de la cpu de 1 solo proceso
    QString out_top = os::sh("sh -c \"echo $(top -b -n 1 -p " +
                             QString::number(pid) + " | tail -n 1)\"");
    QStringList separate = out_top.split(" ");

    int usage = 0;
    if (separate.size() >= 8)
        usage = out_top.split(" ")[8].toFloat();

    return usage;
}

QList<float> ram()
{

    QList<float> resorc;

#if _win32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);
    float total = memInfo.ullTotalPhys / 1048000000.0;
    float free = memInfo.ullAvailPhys / 1048000000.0;
    float used = (total - free);
    float percent = (100 * used) / total;

    resorc.push_back(total);
    resorc.push_back(percent);
#endif

    return resorc;
}

int get_ram_percent(bool cached_percent)
{
    int total, free, buffers, cached, used, percent;

    if (_linux)
    {
        QString mem = os::sh("cat /proc/meminfo");
        total = mem.split("MemTotal:")[1].split("kB")[0].toInt();

        free = mem.split("MemFree:")[1].split("kB")[0].toInt();
        buffers = mem.split("Buffers:")[1].split("kB")[0].toInt();
        cached = mem.split("Cached:")[1].split("kB")[0].toInt();

        used = (total - free) - (buffers + cached);

        if (cached_percent)
            used = (total - free) - used;

        percent = (used * 100) / total;
    }

    else if (_win32)
    {
        percent = ram()[1];
    }

    else
    {
        QString mem = os::sh("memory_pressure");
        percent = 100 - mem.split("percentage:")[1].split("%")[0].toInt();
    }

    return percent;
}

float get_ram_total()
{
    static float total_ram;

    if (not total_ram)
    {
        if (_linux)
        {
            QString mem = os::sh("cat /proc/meminfo");
            long long _total =
                mem.split("MemTotal:")[1].split("kB")[0].toLongLong();
            total_ram = (_total / 1024.0) / 1024.0;
        }
        else if (_win32)
        {
            total_ram = ram()[0];
        }
        else
        {
            QString _ram = os::sh("sysctl hw.memsize");
            _ram = _ram.split("hw.memsize:")[1];

            total_ram = _ram.toLongLong() / 1024 / 1024 / 1024;
        }
    }

    return roundf(total_ram * 10) / 10; // roundf limita los decimales
}

float get_ram_used()
{
    float used = (get_ram_total() * get_ram_percent()) / 100.0;
    return roundf(used * 10) / 10; // roundf limita los decimales
}

int get_cpu_temp()
{
    static int temp;
    if (_linux)
    {
        QString sensors = os::sh("sensors");
        if (not sensors.contains("No sensors found!"))
        {
            int cores = sensors.count("Core ");

            int sumCores = 0;
            for (int i = 0; i < cores; i++)
                sumCores += sensors.split("Core " + QString::number(i) + ":")[1]
                                .split(".")[0]
                                .replace("+", "")
                                .toInt();

            temp = sumCores / cores;
        }
    }

    if (_win32)
    {
        static int csv_delete;
        csv_delete++;
        // Obtiene la temperatura a partir de un sensor con interface core temp,
        // no es lo mas optimo hasta el momento
        QString core_temp_dir = VINARENDER_PATH + "/os/win/core_temp";

        for (QString f : os::listdir(core_temp_dir))
        {
            QString ext = f.split(".").last();
            if (ext == "csv")
            {
                QString _file = fread(f);
                if (not _file.isEmpty())
                {

                    QStringList tempReadList = _file.split("\n");
                    int index = tempReadList.size() - 2;
                    if (index >= 0)
                    {
                        QString tempRead = tempReadList[index];
                        QStringList core = tempRead.split(",,,")[0].split(",");
                        int cpu_count = core.size() - 1;
                        int cores = 0;

                        for (int i = 1; i < cpu_count + 1; ++i)
                            cores += core[i].toInt();

                        if (cores)
                            temp = cores / cpu_count;
                    }
                }

                if (csv_delete > 10)
                {
                    os::remove(f);
                    csv_delete = 0;
                }
            }
        }
    }

    if (_darwin)
    {
        // static int times;

        // if ( times == 0 ){
        // 	QString cmd = path + "/os/mac/sensor/tempmonitor -l -a";
        // 	QString temp_out = os::sh( cmd );

        // 	try{
        // 		temp = stoi( between( temp_out, "SMC CPU A HEAT SINK: ",
        // "C") );
        // 	}
        // 	catch(...){
        // 		try{ temp = stoi( between( temp_out, "SMC CPU A
        // PROXIMITY:
        // ", "C") ); } 		catch(...){}
        // 	}
        // }

        // times++;
        // if ( times == 15 ) times = 0;
    }

    return temp;
}

int get_cpu_cores()
{
    static int cores;

    if (not cores)
    {
        if (_win32)
            cores = os::sh("wmic cpu get NumberOfLogicalProcessors")
                        .split("\n")[1]
                        .toInt();

        else if (_linux)
            cores = os::sh("nproc").toInt();

        else
        {
            QString _cores = os::sh("sysctl hw.ncpu");
            _cores = _cores.split("hw.ncpu:")[1];
            cores = _cores.toInt();
        }
    }

    return cores;
}

}; // namespace hm
