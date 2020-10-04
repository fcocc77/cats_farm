#include "os.hpp"
#include <QDebug>
#include <QString>

namespace os
{

QList<long> getStat()
{
	QList<long> cpu;

	QString s = os::sh("cat /proc/stat");
	QStringList stat = s.split(" ");

	for (int i = 2; i < 10; i++)
		cpu.push_back(stat[i].toLong());

	return cpu;
}

int cpuUsed()
{

	static int usage;
	QString result;
	if (_win32)
	{
		result = sh("C:/Windows/SysWOW64/wbem/wmic.exe cpu get loadpercentage");
		result = result.replace("LoadPercentage", "");

		usage = result.toInt();
	}

	else if (_linux)
	{
		static QList<long> current;
		QList<long> prev;
		long prev_idle, idle, prev_not_idle, not_idle, prev_total, total;
		float totald, idled;

		if (current.empty())
		{
			current = getStat();
			prev = getStat();
		}
		else
		{
			prev = current;
			current = getStat();
		}

		prev_idle = prev[3] + prev[4];
		idle = current[3] + current[4];

		prev_not_idle = prev[0] + prev[1] + prev[2] + prev[5] + prev[6] + prev[7];
		not_idle = current[0] + current[1] + current[2] + current[5] + current[6] + current[7];

		prev_total = prev_idle + prev_not_idle;
		total = idle + not_idle;

		// differentiate: actual value minus the previous one
		totald = total - prev_total;
		idled = idle - prev_idle;

		usage = round(((totald - idled) / totald) * 100.0);
	}

	else
	{
		QString p = os::sh("ps -aeo pcpu | awk '{s+=$1} END {print s }'p");
		usage = p.toInt() / cpuCount();
	}

	return usage;
}

int processCpuUsed(int pid){
	// obtiene el uso de la cpu de 1 solo proceso
	QString out_top = os::sh("sh -c \"echo $(top -b -n 1 -p " + QString::number( pid ) +  " | tail -n 1)\"");
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

void system(QString cmd)
{

	if (_win32)
		cmd = "\"" + cmd + "\"";

	std::system(cmd.toStdString().c_str());
}

float get_ram_total()
{
	static float total_ram;

	if (not total_ram)
	{
		if (_linux)
		{
			QString mem = os::sh("cat /proc/meminfo");
			long long _total = mem.split("MemTotal:")[1].split("kB")[0].toLongLong();
			total_ram = ( _total / 1024.0 ) / 1024.0;
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

int cpuTemp()
{
	static int temp;
	if (_linux)
	{
		QString sensors = sh("sensors");
		if (not sensors.contains("No sensors found!"))
		{
			int cores = sensors.count("Core ");

			int sumCores = 0;
			for (int i = 0; i < cores; i++)
				sumCores += sensors.split("Core " + QString::number(i) + ":")[1].split(".")[0].replace("+", "").toInt();

			temp = sumCores / cores;
		}
	}

	if (_win32)
	{
		static int csv_delete;
		csv_delete++;
		// Obtiene la temperatura a partir de un sensor con interface core temp, no es lo mas optimo hasta el momento
		QString core_temp_dir = path + "/os/win/core_temp";

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
		// 		temp = stoi( between( temp_out, "SMC CPU A HEAT SINK: ", "C") );
		// 	}
		// 	catch(...){
		// 		try{ temp = stoi( between( temp_out, "SMC CPU A PROXIMITY: ", "C") ); }
		// 		catch(...){}
		// 	}
		// }

		// times++;
		// if ( times == 15 ) times = 0;
	}

	return temp;
}

int cpuCount()
{
	static int cores;

	if (not cores)
	{
		if (_win32)
			cores = sh("wmic cpu get NumberOfLogicalProcessors").split("\n")[1].toInt();

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

void copy(QString src, QString dst)
{
	if (QFile::exists(dst))
		QFile::remove(dst);

	QFile::copy(src, dst);
}

void copydir(QString src, QString dst)
{
	os::sh("cp -rf \"" + src + "\" \"" + dst + "\"");
}

void move(QString src, QString dst)
{
	copymove(src, dst, false);
}

void copymove(QString src, QString dst, bool copy)
{

	QString cmd;
	QString cp;
	bool execute = true;

	if (_win32)
	{
		src = src.replace("/", "\\");
		dst = dst.replace("/", "\\");

		if (os::isfile(src))
		{
			if (copy)
				cp = "copy";
			else
				cp = "move";
		}

		else if (os::isdir(src))
		{
			if (copy)
				cp = "echo d | xcopy";
			else
				cp = "move";
		}

		else
		{
			qDebug() << "file or dir not found.";
			execute = false;
		}
	}

	else
	{
		if (os::isfile(src))
		{
			if (copy)
				cp = "cp";
			else
				cp = "mv";
		}
		else if (os::isdir(src))
		{
			if (copy)
				cp = "cp -rf";
			else
				cp = "mv -rf";
		}

		else
		{
			qDebug() << "file or dir not found.";
			execute = false;
		}
	}

	cmd = cp + " \"" + src + "\" \"" + dst + "\"";

	if (execute)
	{
		os::sh(cmd);
	}
}

void remove(QString _file)
{
	QFile file(_file);
	file.remove();
	QDir dir(_file);
	dir.removeRecursively();
}

void rename(QString src, QString dst)
{
	std::rename(src.toStdString().c_str(), dst.toStdString().c_str());
}

QString dirname(QString file)
{
	QFileInfo _file(file);
	return _file.absolutePath();
}

QString basename(QString file)
{
	return file.split("/").last();
}

bool isfile(QString file)
{
	return QFile(file).exists();
}

bool isdir(QString dir)
{
	return QDir(dir).exists();
}

void makedirs(QString dir)
{
	if (!isdir(dir))
		QDir(dir).mkpath(dir);
}

#ifdef _WIN32
void KillProcessTree(DWORD myprocID)
{

	PROCESSENTRY32 pe;

	memset(&pe, 0, sizeof(PROCESSENTRY32));
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (::Process32First(hSnap, &pe))
	{
		do // Recursion
		{
			if (pe.th32ParentProcessID == myprocID)
				KillProcessTree(pe.th32ProcessID);
		} while (::Process32Next(hSnap, &pe));
	}

	// kill the main process
	HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, myprocID);

	if (hProc)
	{
		::TerminateProcess(hProc, 1);
		::CloseHandle(hProc);
	}
}
#endif

void kill(int pid, bool force)
{
	if (_linux)
	{
		vector<int> pids = {pid};
		vector<int> childs = {pid};

		while (1)
		{
			//------------------------------
			for (auto child : childs)
			{
				string result = sh("ps --ppid " + QString::number(child)).toStdString();
				istringstream read(result);
				string line;
				childs = {};
				while (getline(read, line))
				{
					try
					{
						childs.push_back(stoi(line));
					}
					catch (exception &e)
					{
					}
				}
			}
			//------------------------------
			if (not childs.empty())
			{
				for (auto c : childs)
				{
					pids.push_back(c);
				}
			}

			else
			{
				break;
			}
			//------------------------------
		}

		for (auto p : pids)
			if(force)
				sh("kill -9 " + QString::number(p));
			else
				sh("kill " + QString::number(p));
	}

#ifdef _WIN32
	KillProcessTree(pid);
#endif
}

QStringList listdir(QString folder, bool onlyname)
{
	QStringList list_dir;
	QDir ruta = folder;
	QDirIterator it(ruta);

	while (it.hasNext())
	{
		QString file = it.next();
		QString name = basename(file);
		if (onlyname)
			file = name;
		if ((name != ".") and (name != ".."))
			list_dir.push_back(file);
	}

	return list_dir;
}

QString sh(QString cmd, bool error)
{
	QProcess proc;
	proc.start(cmd);
	proc.waitForFinished(-1);
	QString output;
	if (error)
		output = proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
	else
		output = proc.readAllStandardOutput();

	proc.close();

	return output;
}

const QString hostName()
{
	return QHostInfo::localHostName();
}

const QString ip()
{
	QString _ip;
	int i = 0;
	for (auto ip : QNetworkInterface::allAddresses())
	{
		_ip = ip.toString();
		QString first = _ip.split(".")[0];
		if (first == "192")
			return _ip;
		i++;
	}
	return "";
}

const QString mac()
{
	for (QNetworkInterface interface : QNetworkInterface::allInterfaces())
	{
		QString _mac = interface.hardwareAddress();
		QString _ip;

		// encuentra la ip correspondiente a la interface
		QList<QNetworkAddressEntry> address = interface.addressEntries();
		if (!address.empty())
			_ip = address[0].ip().toString();
		// -----------------------

		// retorna la direccion mac que corresponde a la ip
		if (_ip == ip())
			return _mac;
		// ------------------------
	}

	return "";
}

void back(QString cmd)
{
	QProcess pro;
	pro.startDetached(cmd);
	pro.waitForStarted();
}

const QString user()
{

	if (_linux)
	{
		//get user
		QString get_user = "grep '/bin/bash' /etc/passwd | cut -d':' -f1";
		return os::sh(get_user).split("\n")[1];
		//------------------------------------------------------
	}
	else
	{
		return "";
	};
}

} // namespace os