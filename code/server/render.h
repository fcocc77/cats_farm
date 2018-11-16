#ifndef RENDER_H
#define RENDER_H

#include "../utils/util.h"
#include "../utils/threading.h"

class render : public QObject{
public:

	QJsonObject preferences;
	vector <int> first_frame, last_frame, pid;
	vector <QString> project, jobSystem, extra, renderNode, vmSoftware, src_path, dst_path;
	vector <bool> taskKill, renderInstance;
	bool VMCinemaActive;
	bool VMCinemaTurn;
	int VMCinemaRunningTimes;
	QString VMSH;
	
	render(){
		// inicializar instancias 16 veces
		for (int i = 0; i < 15; ++i){
			first_frame.push_back(0);
			last_frame.push_back(0);
			pid.push_back(0);

			taskKill.push_back(false);
			renderInstance.push_back(false);

			project.push_back("none");
			jobSystem.push_back("none");
			extra.push_back("none");
			renderNode.push_back("none");
			vmSoftware.push_back("none");
			src_path.push_back("none");
			dst_path.push_back("none");
		} //-------------------------------------------
	
		threading( &render::suspend_vbox, this );
	}

	QString render_task( QJsonObject recv );
	QString qprocess( QString cmd, int ins = -1 );
	void vbox_turn( bool turn );
	bool vbox_working();	
	void suspend_vbox();
	bool nuke( int ins );
	bool maya( int ins );
	bool houdini( int ins );
	bool cinema( int ins );
	bool fusion( int ins );
	bool noice( int ins );
	bool ffmpeg( int ins );

};

#endif // RENDER_H
