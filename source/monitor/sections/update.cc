// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QProgressBar>

#include "../global/global.h"
#include "jobs_struct.h"
#include "servers_struct.h"
#include "threading.h"
#include "update.h"
#include "util.h"

update_class::update_class(shared_variables *_shared, groups_class *_groups,
                           jobs_class *_jobs, servers_class *_servers,
                           QTreeWidget *_tasks_tree, settings_class *_settings)

    : shared(_shared)
    , groups(_groups)
    , jobs(_jobs)
    , servers(_servers)
    , tasks(_tasks_tree)
    , settings(_settings)
{
    get_task_thread = new QThread();
}

void update_class::update(QString host)
{
    // mata la actualizacion solo si el update inicializo una vez,
    // esto es para que no crashee si es que el manager no esta inicializado.
    if (update_started)
        manager->kill();

    // guarda la zona actual en settings, para que cuando
    // abramos nuevamente el vmonitor inicie con esa zona
    shared->settings["current_manager"] = host;
    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);

    shared->manager_host = host;

    groups->get_tree()->clear();
    servers->get_tree()->clear();
    jobs->get_tree()->clear();
    tasks->clear();

    // nmap checkea si el puerto esta esta abierto, si no retorna, esto
    // se hace para que no se quede pegado en el tcpClient
    shared->conection->setText("");

    if (_linux)
    {
        QString nmap =
            os::sh("nmap -Pn " + host + " -p " + QString::number(MANAGER_PORT));

        if (!nmap.contains("open"))
        {
            shared->conection->setText("Connection failed: " + host);
            return;
        }
    }

    // actualiza los datos de settings dependiendo de la zona
    settings->get_manager()->update(host);

    // actualiza una ves antes del loop.
    QString recv = tcpClient(host, MANAGER_PORT, jats({2, "none"}));
    manager_recieve_update(recv, {});

    manager = tcpClient(host, MANAGER_PORT,
                        &update_class::manager_recieve_update, this, true);
    update_started = true;
}

QString update_class::manager_recieve_update(QString _recv, QJsonObject extra)
{
    QJsonObject recv = jofs(_recv);

    static int timelapse;
    if (not recv.empty())
    {
        if (not shared->stopUpdate)
        {
            shared->jobs = recv["jobs"].toObject();
            shared->servers = recv["servers"].toObject();

            update_jobs(recv["jobs"].toObject());
            update_servers(recv["servers"].toObject());
            update_groups(recv["groups"].toObject());
        }

        else
        {
            // va sumando segundos, y si es mayor a 3 seg, se puede actualizar
            // otra vez; esto es para que cuando se borre un item, no vuelva a
            // aparecer y desaparecer
            timelapse++;
            if (timelapse > 3)
            {
                shared->stopUpdate = false;
                timelapse = 0;
            }
        }
    }

    return jats({2});
}

void update_class::update_jobs(QJsonObject recv)
{
    QStringList job_list;

    struct _job_item
    {
        QString name;
        QTreeWidgetItem *item;
    };
    QList<_job_item> job_item;

    for (int i = 0; i < jobs->get_tree()->topLevelItemCount(); ++i)
    {
        auto item = jobs->get_tree()->topLevelItem(i);
        QString name = item->text(0);
        job_list.push_back(name);
        job_item.push_back({name, item});
    }

    // actualiza el contador de jobs de toolbar
    int count = recv.size();
    shared->jobs_count->setText(QString::number(count) + " jobs.");

    QStringList job_name_list;
    for (QJsonValue j : recv)
    {
        QJsonObject job = j.toObject();
        QString job_name = job["name"].toString();
        QString status = job["status"].toString();
        int errors = job["errors"].toInt();
        QString submit_start = job["submit_start"].toString();
        QString submit_finish = job["submit_finish"].toString();
        QString total_render_time = job["total_render_time"].toString();
        QString estimated_time = job["estimated_time"].toString();
        QString software = job["software"].toString();
        int _tasks = job["tasks"].toInt();
        int progres = job["progres"].toInt();

        int porcent = (progres * 100) / _tasks;

        // priority text
        QString priority;

        int p = job["priority"].toInt();
        ;

        if (p == 0)
            priority = "Very High";
        else if (p == 1)
            priority = "High";
        else if (p == 2)
            priority = "Normal";
        else if (p == 3)
            priority = "Low";
        else
            priority = "Very Low";

        // si el jobs se borro recientemente no se agrega ni actualiza
        bool _delete = false;
        for (QString d : delete_list)
            if (d == job_name)
                _delete = true;

        // agrega a la lista para saber que jobs ya no estan
        job_name_list.push_back(job_name);

        if (not _delete)
        {
            // Si el nombre no esta en la lista de la interface, agrega el nuevo
            // job.
            if (not job_list.contains(job_name))
            {
                QTreeWidgetItem *item = new QTreeWidgetItem();

                QProgressBar *taskProgress = new QProgressBar();
                taskProgress->setValue(porcent);
                taskProgress->setMaximumHeight(17);
                taskProgress->setObjectName("taskProgress");

                QVBoxLayout *vbox = new QVBoxLayout();
                vbox->addWidget(taskProgress);
                vbox->setContentsMargins(0, 0, 0, 0);
                QWidget *widget = new QWidget();
                widget->setLayout(vbox);

                item->setText(0, job_name);
                item->setText(1, priority);
                item->setText(2, software);

                item->setText(4, status);
                item->setText(5, submit_start);
                item->setText(6, submit_finish);
                item->setText(7, estimated_time);

                item->setText(8, total_render_time);
                item->setText(9, QString::number(errors));

                jobs->get_tree()->addTopLevelItem(item);
                jobs->get_tree()->setItemWidget(item, 3, widget);
            }

            for (int i = 0; i < jobs->get_tree()->topLevelItemCount(); ++i)
            {
                auto item = jobs->get_tree()->topLevelItem(i);
                QString name = item->text(0);
                if (name == job_name)
                {
                    item->setText(0, job_name);
                    item->setText(1, priority);
                    item->setText(2, software);

                    QProgressBar *taskProgress =
                        jobs->get_tree()->itemWidget(item, 3)->findChild<QProgressBar *>();

                    taskProgress->setValue(porcent);
                    taskProgress->setFormat(QString::number(porcent) + "%   (" +
                                            QString::number(progres) + "/" +
                                            QString::number(_tasks) + ")");

                    item->setText(4, status);
                    item->setText(5, submit_start);
                    item->setText(6, submit_finish);
                    item->setText(7, estimated_time);
                    item->setText(8, total_render_time);
                    item->setText(9, QString::number(errors));

                    if (status == "Completed")
                    {
                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(70, 100, 120);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(80, 150, 200));
                    }

                    if (status == "Rendering...")
                    {
                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(50, 120, 70);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(100, 200, 100));
                    }

                    if (status == "Failed")
                    {

                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(200, 0, 0);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(200, 0, 0));
                    }

                    if (status == "Concatenate")
                    {
                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(120, 100, 120);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);
                        taskProgress->setFormat("....Concat....");

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(180, 130, 200));
                    }

                    if (status == "Queue")
                    {
                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(140, 140, 140);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(200, 200, 200));
                    }

                    if (status == "Paused")
                    {
                        QString bar_color = "QProgressBar::chunk:horizontal "
                                            "{background: rgb(140, 140, 0);}";
                        jobs->get_tree()->itemWidget(item, 3)->setStyleSheet(bar_color);

                        for (int i = 0; i < 10; ++i)
                            item->setForeground(i, QColor(200, 200, 0));
                    }
                }
            }
        }
    }
    // si el job ya no existe, borra el item
    for (_job_item ji : job_item)
    {
        if (not job_name_list.contains(ji.name))
        {
            auto root = jobs->get_tree()->invisibleRootItem();
            root->removeChild(ji.item);
        }
    }
}

void update_class::update_servers(QJsonObject recv)
{

    // Chekear lista de server que hay en la lista
    QStringList server_list;
    struct _server_item
    {
        QString name;
        QTreeWidgetItem *item;
    };
    QList<_server_item> server_item;

    for (int i = 0; i < servers->get_tree()->topLevelItemCount(); ++i)
    {
        auto item = servers->get_tree()->topLevelItem(i);
        QString name = item->text(0);
        server_list.push_back(name);
        server_item.push_back({name, item});
    }

    // crea lista de los server que no estan filtrados
    QStringList displayServer;
    for (QJsonValue s : recv)
    {
        QJsonObject server = s.toObject();
        QString server_name = server["name"].toString();
        QString status = server["status"].toString();
        QString system = server["system"].toString();

        bool display = false;
        if (status == "absent")
        {
            if (shared->server_display["off"].toBool())
            { // OFF
                display = true;
            }
        }

        else
        {
            if (shared->server_display["on"].toBool())
            { // ON
                display = true;
            }
        }

        if (display)
        {
            if (system == "Windows")
            {
                if (shared->server_display["window"].toBool())
                { // Windows
                    displayServer.push_back(server_name);
                }
            }

            if (system == "Linux")
            {
                if (shared->server_display["linux"].toBool())
                { // Linux
                    displayServer.push_back(server_name);
                }
            }

            if (system == "Mac")
            {
                if (shared->server_display["mac"].toBool())
                { // Mac
                    displayServer.push_back(server_name);
                }
            }
        }
    }

    QStringList server_name_list;

    for (QJsonValue s : recv)
    {
        QJsonObject server = s.toObject();
        QString server_name = server["name"].toString();
        int max_instances = server["max_instances"].toInt();
        QString status = server["status"].toString();
        QString ip = server["ip"].toString();
        QString system = server["system"].toString();
        int cpu_used = server["cpu_used"].toInt();
        int cpu_iowait = server["cpu_iowait"].toInt();
        int ram_usage_percent = server["ram_usage_percent"].toInt();
        int ram_cache_percent = server["ram_cache_percent"].toInt();
        int cpu_temp = server["cpu_temp"].toInt();
        QString mac_address = server["mac_address"].toString();
        float ram_total = server["ram_total"].toDouble();
        float ram_used = server["ram_used"].toDouble();
        int cpu_cores = server["cpu_cores"].toInt();

        QList<inst_struct *> instances;
        for (QJsonValue i : server["instances"].toArray())
        {
            QJsonArray ins = i.toArray();
            inst_struct *_ins = new inst_struct;
            *_ins = {ins[0].toInt(), ins[1].toInt(), ins[2].toBool(),
                     ins[3].toString()};
            instances.push_back(_ins);
        }

        // crea lista con el nombre de la instancia y el status
        QString instance_job;
        // 0 = idle,	1 = active,		2 = disable,	3 = failed
        QList<bool> general_status = {0, 0, 0, 0};
        int active_instances = 0;

        if (status == "absent")
            instance_job = "...";

        else
        {
            for (int i = 0; i < max_instances; ++i)
            {
                if (instances[i]->status == 1)
                    active_instances += 1;
                general_status[instances[i]->status] = true;
                instance_job += "   " + instances[i]->job_task;
            }

            if (general_status[3])
                status = "failed";

            else if (general_status[2])
                status = "disabled";

            else if (general_status[1])
                status = "active";

            else
                status = "idle";
        }

        // si el jobs se borro recientemente no se agrega ni actualiza
        bool _delete = false;
        for (auto d : delete_list)
            if (d == server_name)
                _delete = true;


        if (not _delete)
        {
            // a veces cuando lee el archivo de server en el manager, no hay
            // info y queda como None si el server name no igual a None crea el
            // item

            if (displayServer.contains(server_name))
            {

                // agrega a la lista para saber que server ya no estan
                server_name_list.push_back(server_name);

                // Si el nombre no esta en la lista de la interface, agrega el
                // nuevo job.
                if (not server_list.contains(server_name))
                {

                    progress_bar_class *cpu_bar = new progress_bar_class();
                    cpu_bar->set_color(50, 80, 100, 1.3);
                    cpu_bar->setObjectName("cpu_bar");
                    QVBoxLayout *cpu_vbox = new QVBoxLayout();
                    cpu_vbox->addWidget(cpu_bar);
                    cpu_vbox->setContentsMargins(0, 0, 0, 0);
                    QWidget *cpu_widget = new QWidget();
                    cpu_widget->setLayout(cpu_vbox);

                    progress_bar_class *ram_bar = new progress_bar_class();
                    ram_bar->set_color(10, 130, 70);
                    ram_bar->setObjectName("ram_bar");
                    QVBoxLayout *ram_vbox = new QVBoxLayout();
                    ram_vbox->addWidget(ram_bar);
                    ram_vbox->setContentsMargins(0, 0, 0, 0);
                    QWidget *ram_widget = new QWidget();
                    ram_widget->setLayout(ram_vbox);

                    QTreeWidgetItem *item = new QTreeWidgetItem();

                    item->setText(0, server_name);
                    item->setText(1, status);
                    item->setText(2, QString::number(max_instances));
                    item->setText(4, QString::number(cpu_temp));
                    item->setText(6, system);
                    item->setText(7, ip);
                    item->setText(8, mac_address);
                    item->setText(9, instance_job);

                    item->setForeground(8, QColor(100, 150, 200));

                    item->setTextAlignment(4, Qt::AlignCenter);

                    servers->get_tree()->addTopLevelItem(item);

                    servers->get_tree()->setItemWidget(item, 3, cpu_widget);
                    servers->get_tree()->setItemWidget(item, 5, ram_widget);
                }

                for (int i = 0; i < servers->get_tree()->topLevelItemCount(); ++i)
                {

                    auto item = servers->get_tree()->topLevelItem(i);
                    QString name = item->text(0);
                    if (name == server_name)
                    {

                        if ((active_instances > 1) and (status == "active"))
                            item->setText(1, QString::number(active_instances) +
                                                 " " + status);
                        else
                            item->setText(1, status);

                        item->setText(0, server_name);
                        item->setText(2, QString::number(max_instances));
                        item->setText(7, ip);
                        item->setText(9, instance_job);
                        item->setText(4, QString::number(cpu_temp) + "°C");

                        progress_bar_class *cpu_bar =
                            servers->get_tree()
                                ->itemWidget(item, 3)
                                ->findChild<progress_bar_class *>();

                        progress_bar_class *ram_bar =
                            servers->get_tree()
                                ->itemWidget(item, 5)
                                ->findChild<progress_bar_class *>();

                        cpu_bar->set_text(QString::number(cpu_used) + "% (" +
                                          QString::number(cpu_cores) +
                                          " Cores)");
                        ram_bar->set_text(QString::number(ram_usage_percent) + "%  (" +
                                          QString::number(ram_used) + " GB / " +
                                          QString::number(ram_total) + " GB)");

                        if (status == "absent")
                            item->setForeground(4, QColor(200, 200, 200));
                        else if (cpu_temp < 60)
                            item->setForeground(4, QColor(100, 200, 100));
                        else if (cpu_temp < 85)
                            item->setForeground(4, QColor(200, 200, 0));
                        else
                            item->setForeground(4, QColor(200, 0, 0));

                        if (status == "absent")
                        {
                            item->setForeground(0, QColor(255, 30, 30));
                            item->setForeground(1, QColor(255, 30, 30));

                            cpu_bar->set_value(0, 0);
                            ram_bar->set_value(0, 0);
                            cpu_bar->set_text("...");
                            item->setText(4, "...");
                            ram_bar->set_text("...");
                        }

                        else
                        {
                            cpu_bar->set_value(cpu_used, cpu_iowait - cpu_used);
                            ram_bar->set_value(ram_usage_percent, ram_cache_percent);

                            if (general_status[3])
                            {
                                item->setForeground(0, QColor(200, 0, 0));
                                item->setForeground(1, QColor(200, 0, 0));
                            }
                            else if (general_status[1])
                            {
                                item->setForeground(0, QColor(100, 200, 100));
                                item->setForeground(1, QColor(100, 200, 100));
                            }
                            else if (general_status[2])
                            {
                                item->setForeground(0, QColor(200, 200, 0));
                                item->setForeground(1, QColor(200, 200, 0));
                            }
                            else
                            {
                                item->setForeground(0, QColor(200, 200, 200));
                                item->setForeground(1, QColor(200, 200, 200));
                            }
                        }
                    }
                }
            }
        }
    }

    // si el server ya no existe, o esta oculto borra el item
    for (auto si : server_item)
    {
        if (not server_name_list.contains(si.name))
        {
            auto root = servers->get_tree()->invisibleRootItem();
            root->removeChild(si.item);
        }
    }
    //--------------------------------------------
}

void update_class::update_groups(QJsonObject recv)
{

    if (not recv.empty())
    {
        QStringList group_list;

        struct _group_item
        {
            QString name;
            QTreeWidgetItem *item;
        };
        QList<_group_item> group_item;
        for (int i = 0; i < groups->get_tree()->topLevelItemCount(); ++i)
        {
            auto item = groups->get_tree()->topLevelItem(i);
            QString name = item->text(2);

            group_list.push_back(name);
            group_item.push_back({name, item});
        }

        for (QJsonValue g : recv)
        {
            QJsonObject group = g.toObject();
            QString group_name = group["name"].toString();
            bool group_status = group["status"].toBool();
            int totaMachine = group["totaMachine"].toInt();
            int activeMachine = group["activeMachine"].toInt();
            int offMachine = totaMachine - activeMachine;
            QJsonArray machines = group["server"].toArray();

            // Si el nombre no esta en la lista de la interface, agrega el nuevo
            // grupo.
            if (not group_list.contains(group_name))
                groups->group_make(group_name, totaMachine, activeMachine,
                                   offMachine);

            //-----------------------------------------------------------------------
            for (int i = 0; i < groups->get_tree()->topLevelItemCount(); ++i)
            {

                auto item = groups->get_tree()->topLevelItem(i);
                QString name = item->text(2);
                if (name == group_name)
                {
                    groups->make_server(item, machines);

                    item->setText(2, group_name);

                    QList<QLabel *> labels =
                        groups->get_tree()->itemWidget(item, 1)->findChildren<QLabel *>();
                    QList<QLabel *> name_label =
                        groups->get_tree()->itemWidget(item, 0)->findChildren<QLabel *>();

                    labels[1]->setText(QString::number(activeMachine));
                    labels[3]->setText(QString::number(offMachine));
                    labels[5]->setText(QString::number(totaMachine));

                    if (group_status)
                        name_label[1]->setText("Group is being used");
                    else
                        name_label[1]->setText("Group is idle");

                    if (activeMachine)
                    {
                        labels[1]->setStyleSheet(
                            "QLabel{color: rgb(100,200,100);}");
                        name_label[0]->setStyleSheet(
                            "QLabel{color: rgb(220,220,220);  font:10pt;}");
                        if (group_status)
                            name_label[1]->setStyleSheet(
                                "QLabel{color: rgb(100,200,100);  font:8pt;}");
                        else
                            name_label[1]->setStyleSheet(
                                "QLabel{color: rgb(100,150,220);  font:8pt;}");
                    }
                    else
                    {
                        labels[1]->setStyleSheet(
                            "QLabel{color: rgb(255,30,30);}");
                        name_label[0]->setStyleSheet(
                            "QLabel{color: rgb(255,30,30);  font:10pt;}");
                        name_label[1]->setStyleSheet(
                            "QLabel{color: rgb(255,30,30);  font:8pt;}");
                    }

                    if (not offMachine)
                        labels[3]->setStyleSheet(
                            "QLabel{color: rgb(100,200,100);}");
                    else
                        labels[3]->setStyleSheet(
                            "QLabel{color: rgb(255,30,30);}");

                    labels[5]->setStyleSheet(
                        "QLabel{color: rgb(120,150,250);}");
                }
            }
        }
    }
    else
    {
        groups->get_tree()->clear();
    }
}

void update_class::get_task()
{
    task_first_add = false;

    if (get_task_thread->isRunning())
        get_task_thread->terminate();

    tasks->clear();

    update_task(); // actualiza una vez antes del hilo
    get_task_thread = qthread(&update_class::update_task, this);
}

void update_class::update_task()
{
    auto selected = jobs->get_tree()->selectedItems();
    if (not selected.empty())
    {
        auto _item = selected[0];
        QString job_item_name = _item->text(0);
        QList<QStringList> _tasks;

        // encuentra en los paketes recibidos el job seleccionado
        QJsonObject job;
        for (QJsonValue _job : shared->jobs)
        {
            if (_job.toObject()["name"].toString() == job_item_name)
            {
                job = _job.toObject();
                break;
            }
        }

        for (QJsonValue t : job["task"].toArray())
        {
            QJsonObject task = t.toObject();

            QString frameRange = QString::number(task["first_frame"].toInt()) +
                                 "-" +
                                 QString::number(task["last_frame"].toInt());

            _tasks.push_back(
                {task["name"].toString(), frameRange, task["status"].toString(),
                 task["server"].toString(), task["time"].toString()});
        }

        auto item_set = [=](QTreeWidgetItem *item, QString taskName,
                            QString frameRange, QString status, QString server,
                            QString tiempo) {
            item->setText(0, taskName);
            item->setText(1, frameRange);
            item->setText(2, status);
            item->setText(3, server);
            item->setText(4, tiempo);

            if (status == "active")
            {
                for (int i = 0; i < 5; ++i)
                {
                    item->setForeground(i, QColor(100, 200, 100));
                }
            }

            if (status == "finished")
            {
                for (int i = 0; i < 5; ++i)
                {
                    item->setForeground(i, QColor(80, 150, 200));
                }
            }

            if (status == "waiting")
            {
                for (int i = 0; i < 5; ++i)
                {
                    item->setForeground(i, QColor(200, 200, 200));
                }
            }

            if (status == "paused")
            {
                for (int i = 0; i < 5; ++i)
                {
                    item->setForeground(i, QColor(200, 200, 0));
                }
            }

            if (status == "failed")
            {
                for (int i = 0; i < 5; ++i)
                {
                    item->setForeground(i, QColor(200, 0, 0));
                }
            }
        };

        if (not task_first_add)
        {

            for (QStringList task : _tasks)
            {

                QString taskName = task[0], frameRange = task[1],
                        status = task[2], server = task[3], tiempo = task[4];

                QTreeWidgetItem *item = new QTreeWidgetItem();
                item_set(item, taskName, frameRange, status, server, tiempo);

                tasks->addTopLevelItem(item);

                task_first_add = true;
            }
        }

        else
        {

            for (QStringList task : _tasks)
            {
                QString taskName = task[0], frameRange = task[1],
                        status = task[2], server = task[3], tiempo = task[4];

                for (int i = 0; i < tasks->topLevelItemCount(); ++i)
                {
                    auto item = tasks->topLevelItem(i);
                    QString name = item->text(0);

                    if (name == taskName)
                    {
                        item_set(item, taskName, frameRange, status, server,
                                 tiempo);
                        break;
                    }
                }
            }
        }
    }

    else
    {
        tasks->clear();
    }
}
