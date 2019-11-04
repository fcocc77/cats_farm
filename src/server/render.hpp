#ifndef RENDER_HPP
#define RENDER_HPP

#include "../utils/util.hpp"
#include "../utils/threading.hpp"

class render : public QObject
{
  public:
	QJsonObject preferences;
	QList<int> first_frame, last_frame, pid;
	QList<QString> project, jobSystem, extra, renderNode, vmSoftware, src_path, dst_path;
	QList<bool> taskKill, renderInstance;
	bool VMCinemaActive;
	bool VMCinemaTurn;
	int VMCinemaRunningTimes;
	QString VMSH;

	render()
	{
		// inicializar instancias 16 veces
		for (int i = 0; i < 15; ++i)
		{
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

		threading(&render::suspend_vbox, this);
	}

	QString render_task(QJsonArray recv);
	QList <QString> find_correct_path(QJsonArray system_path, QString _path);
	QString qprocess(QString cmd, int ins = -1);
	void vbox_turn(bool turn);
	bool vbox_working();
	void suspend_vbox();
	bool nuke(int ins);
	bool maya(int ins);
	bool houdini(int ins);
	bool cinema(int ins);
	bool fusion(int ins);
	bool ae(int ins);
};

#endif // RENDER_HPP
