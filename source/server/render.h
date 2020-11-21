#ifndef RENDER_HPP
#define RENDER_HPP

#include <util.h>
#include "../utils/threading.h"
#include <QMutex>

class render_class : public QObject
{
public:
	QJsonObject preferences;
	QList<int> first_frame, last_frame, pid;
	QList<QString> project, jobSystem, extra, renderNode, vmSoftware, src_path, dst_path;
	QList<bool> taskKill, renderInstance;
	QMutex *mutex;

	render_class(QMutex *_mutex);

	QString render_task(QJsonArray recv);
	QList<QString> find_correct_path(QString file_path);
	QString qprocess(QString cmd, int ins = -1, int timeout = -1);
	bool nuke(int ins);
	bool maya(int ins);
	bool houdini(int ins);
	bool ntp(int ins);
	void natron_monitoring(int ins);
	bool natron(int ins);
	bool ae(int ins);
};

#endif // RENDER_HPP
