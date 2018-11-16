#include "os.h"
#include <QDebug>
#include <QString>

namespace os {

	vector <int> getStat(){
		vector <int> cpu;
		QString s = fread("/proc/stat");

		auto stat = split(s , " ");

		for ( int i=2; i<10; i++){

			try{ cpu.push_back( stoi( stat[i] ) );}
			catch( exception &e ){}

		}

		return cpu;
	}

	int cpuUsed(){

		static int usage;
		QString result;
		if ( _win32 ){
			result = sh("wmic cpu get loadpercentage");
			result = replace(result, "LoadPercentage", "");

			try { usage = stoi(result); }
			catch( exception &e ){}
		}

		else if ( _linux ){

			int prev_idle, idle, prev_not_idle, not_idle, prev_total, total;
			float totald, idled;

			auto prev=getStat();
			usleep(100000);
			auto current=getStat();

			prev_idle = prev[3] + prev[4];
			idle = current[3] + current[4];

			prev_not_idle = prev[0] + prev[1] + prev[2] + prev[5] + prev[6] + prev[7];
			not_idle = current[0] + current[1] + current[2] + current[5] + current[6] + current[7];

			prev_total = prev_idle + prev_not_idle;
			total = idle + not_idle;

			// differentiate: actual value minus the previous one
			totald = total - prev_total;
			idled = idle - prev_idle;

			usage = round( ( ( totald - idled ) / totald ) * 100.0 );

		}

		else{
			QString p = os::sh( "ps -aeo pcpu | awk '{s+=$1} END {print s }'p" );
			try{ usage = stoi(p) / cpuCount();}
			catch(...){}

		}

		return usage;
	}

	vector <float> ram(){

		vector <float> resorc;

		#if _win32
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(memInfo);
			GlobalMemoryStatusEx(&memInfo);
			float total = memInfo.ullTotalPhys/1048000000.0;
			float free =  memInfo.ullAvailPhys/1048000000.0;
			float used = (total-free);
			float percent=(100*used)/total;

			resorc.push_back(total);
			resorc.push_back(percent);
		#endif

		return resorc;
	}

	int ramPercent(){
		int total, free, buffers, cached, used, percent;

		if ( _linux ){
			QString mem=fread("/proc/meminfo");
			total=stoi(between(mem, "MemTotal:", "kB"));
			free=stoi(between(mem, "MemFree:", "kB"));
			buffers=stoi(between(mem, "Buffers:", "kB"));
			cached=stoi(between(mem, "Cached:", "kB"));
			used=(total-free)-(buffers+cached);
			percent=(used*100)/total;
		}

		else if ( _win32 ){
			percent = ram()[1];

		}

		else {
			QString mem = os::sh( "memory_pressure" );
			percent = 100 - stoi( between( mem, "percentage:", "%") );
		}

		return percent;
	}

	void system( QString cmd ){

		if ( _win32 ) cmd = "\"" + cmd + "\"";

		std::system( cmd.c_str() );
	}

	int ramTotal(){

		static int total_ram;

		if ( not total_ram ){
			if ( _linux ){
				QString mem=fread("/proc/meminfo");
				long long _total = stoll(between(mem, "MemTotal:", "kB"));
				total_ram = ( _total * 1024 * 1024 ) / 1000000000000;

			}
			else if ( _win32 ){
				total_ram = ram()[0];
			}

			else{
				QString _ram = os::sh( "sysctl hw.memsize");
				_ram = split(_ram, "hw.memsize:")[1];

				try{ total_ram = stoll(_ram) / 1024 / 1024 / 1024;}
				catch(...){}

			}

		}

		return total_ram;
	}

	float ramUsed(){
		float used = ( ramTotal() * ramPercent() ) / 100.0; 
		return roundf( used * 10 ) / 10; // roundf limita los decimales
	}

	int cpuTemp(){
		static int temp;
		if ( _linux ){

			try{
				QString t = sh("sensors");
				int pos, core0, core1, core2, core3;
				//core0
				pos = t.find("Core 0:");  t.erase(0,pos+7); 
				pos = t.find("+"); t.erase(0,pos+1);
				pos = t.find("°C"); core0=stoi(t.substr(0,pos+2));
				//core1
				pos = t.find("Core 1:");  t.erase(0,pos+7); 
				pos = t.find("+"); t.erase(0,pos+1);
				pos = t.find("°C"); core1=stoi(t.substr(0,pos+2));
				//core2
				pos = t.find("Core 2:");  t.erase(0,pos+7); 
				pos = t.find("+"); t.erase(0,pos+1);
				pos = t.find("°C"); core2=stoi(t.substr(0,pos+2));
				//core3
				pos = t.find("Core 3:");  t.erase(0,pos+7); 
				pos = t.find("+"); t.erase(0,pos+1);
				pos = t.find("°C"); core3=stoi(t.substr(0,pos+2));
				//------------------------------------------

				temp = (core0+core1+core2+core3)/4;
			}
			catch (exception& e){
			}
		}

		if ( _win32 ){
			static int csv_delete;
			csv_delete++;
			// Obtiene la temperatura a partir de un sensor con interface core temp, no es lo mas optimo hasta el momento
			QString core_temp_dir = path() + "/os/win/core_temp";

			for ( QString f : os::listdir( core_temp_dir ) ){
				QString ext = f.substr( f.length() - 3 );
				if ( ext == "csv" ){
					QString _file = fread( f );
					if ( not _file.empty() ){

						QString tempRead = split( fread( f ), "\n" ).end()[-1] ;

						auto core = split( split( tempRead, ",,," )[0], "," );
						int cpu_count = core.size() - 1;
						int cores = 0;

						for ( int i = 1; i < cpu_count + 1; ++i ){
							try{ cores += stoi( core[i] ); } // puede dar error si no es numero
							catch(...){}
						}

						if ( cores ) temp = cores/cpu_count;
					}

					if ( csv_delete > 10 ){
						os::remove(f);
						csv_delete = 0;
					}
				}
			}
		}

		if ( _darwin ){
			static int times;
			
			if ( times == 0 ){ 
				QString cmd = path() + "/os/mac/sensor/tempmonitor -l -a";
				QString temp_out = os::sh( cmd );

				try{ 
					temp = stoi( between( temp_out, "SMC CPU A HEAT SINK: ", "C") );
				}
				catch(...){
					try{ temp = stoi( between( temp_out, "SMC CPU A PROXIMITY: ", "C") ); }
					catch(...){}
				}
			}
			
			times++;
			if ( times == 15 ) times = 0; 
		}

		return temp;
	}

	int cpuCount(){
		static int cores;

		if ( not cores ){
			if ( _win32 ){
				try { cores = stoi( sh( "echo %NUMBER_OF_PROCESSORS%" ) ); }
				catch( exception &e){}
			}

			else if ( _linux ){
				try{ cores = stoi( os::sh("nproc") );}
				catch(...){}

			}

			else {
				QString _cores = os::sh( "sysctl hw.ncpu" );
				_cores = split( _cores, "hw.ncpu:" )[1];

				try{ cores = stoi(_cores);}
				catch(...){}
			}
		}

		return cores;
	}

	void copy( QString src, QString dst ){
		copymove( src, dst, true );
	}

	void move( QString src, QString dst ){
		copymove( src, dst, false );
	}

	void copymove( QString src, QString dst, bool copy ){

		QString cmd;
		QString cp;
		bool execute = true;

		if ( _win32 ){
			src = replace( src, "/", "\\" );
			dst = replace( dst, "/", "\\" );

			if( os::isfile( src ) ){
				if ( copy ) cp = "copy";
				else cp = "move";
			}

			else if( os::isdir( src ) ){
				if ( copy ) cp = "echo d | xcopy";
				else cp = "move";
			}

			else{  
				qDebug() << "file or dir not found.";  		
				execute = false;
			}
		}

		else{
			if( os::isfile( src ) ){
				if ( copy ) cp = "cp";
				else cp = "mv";
			}
			else if( os::isdir( src ) ){
				if ( copy ) cp = "cp -rf";
				else cp = "mv -rf";
			}

			else{  
				qDebug() << "file or dir not found.";  		
				execute = false;
			}
		}

		cmd = cp + " \"" + src + "\" \"" + dst + "\"";

		if ( execute ){ 
			os::sh( cmd );
		}
	}

	void mkdir ( QString path ){
		QString cmd = "mkdir \"" + path + '"';
		if ( _win32 ) cmd = replace( cmd, "/", "\\" );
		std::system( cmd.c_str() );
	}

	void remove( QString _file ){
		QFile file ( QString::fromStdString(_file) );
		file.remove();
		QDir dir( QString::fromStdString(_file) );
		dir.removeRecursively();	
	}

	void rename( QString src, QString dst ){
		std::rename( src.c_str(), dst.c_str() );
	}

	QString dirname( QString file ){
		size_t found;
		found=file.find_last_of("/\\");
		return file.substr(0,found);
	}

	QString basename( QString file ){
		size_t found;
		found=file.find_last_of("/\\");
		return file.substr(found+1);
	}

	bool isfile( QString file ){
		return QFile( QString::fromStdString( file ) ).exists();
	}

	bool isdir( QString dir ){
		return QDir( QString::fromStdString( dir ) ).exists();
	}

	#ifdef _WIN32
	void KillProcessTree( DWORD myprocID ){

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
			} 
			while (::Process32Next(hSnap, &pe));
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

	void kill( int pid ){
		if ( _linux ){
			vector <int> pids = {pid};
			vector <int> childs = {pid};

			while (1){
				//------------------------------
				for ( auto child : childs ){
					QString result = sh( "ps --ppid " + to_string(child));
					istringstream read( result );  
					QString line; 
					childs = {};
					while ( getline( read, line )){
						try { childs.push_back( stoi( line ) ); }
						catch ( exception& e ){}
					}	
				}
				//------------------------------
				if ( not childs.empty() ){ 
					for ( auto c : childs ){
						pids.push_back(c);
					}
				}

				else { break; }
				//------------------------------
			}

			for ( auto p : pids ){
				sh( "kill " + to_string(p) );
			}
		}

		#ifdef _WIN32
			KillProcessTree(pid);
		#endif
	}

	vector <QString> listdir( QString folder, bool onlyname ){
		vector <QString> list_dir;
		QDir ruta = QString::fromStdString(folder);
		QDirIterator it(ruta);

		while (it.hasNext()){ 
			QString file = it.next().toStdString();
			QString name = basename( file );
			if ( onlyname ) file = name;
			if ( ( name != "." ) and ( name != ".." ) ) list_dir.push_back( file );
		}

		return list_dir;
	}

	QString sh(QString cmd) {

		QString data;
		FILE * stream;
		const int max_buffer = 256;
		char buffer[max_buffer];
		cmd.append(" 2>&1");

		if ( _win32 ) cmd = "\"" + cmd + "\"";
		stream = popen(cmd.c_str(), "r");
		if (stream) {
			while (!feof(stream))
		if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
		return data;
	}

	const QString hostName(){
		return QHostInfo::localHostName().toStdString();
	}

	const QString ip(){
		QString _ip;
		int i = 0;
		for ( auto ip : QNetworkInterface::allAddresses() ){
			_ip = ip.toString().toStdString();
			QString first = split( _ip, "." )[0];
			if ( first == "192" ) return _ip ;
			i++;
		}
		return "";
	}

	void back( QString cmd ){
		QProcess pro;
		pro.startDetached(QString::fromStdString(cmd) );
		pro.waitForStarted();
	}

	const QString user(){

		if ( _linux ){
			//get user
			QString get_user = "grep '/bin/bash' /etc/passwd | cut -d':' -f1";                
			return split( os::sh(get_user) , "\n" )[1];
			//------------------------------------------------------
		}
		else { return ""; };
	}

	QString qp( QString cmd ){
		QProcess proc;
		proc.start( QString::fromStdString(cmd) );
		proc.waitForFinished(-1);
		QString output = proc.readAllStandardOutput();
		proc.close();

		return output.toStdString();
	}
}