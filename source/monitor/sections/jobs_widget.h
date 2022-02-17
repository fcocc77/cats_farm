// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef JOBS_HPP
#define JOBS_HPP

#include <QTreeWidget>

#include "groups_widget.h"
#include "log.h"
#include "options.h"
#include "properties.h"
#include "servers_widget.h"
#include "submit_widget.h"
#include "title_bar.h"

class jobs_class : public QWidget
{
private:
    shared_variables *shared;
    QWidget *_monitor;
    log_class *log_widget;
    servers_class *servers;
    options_class *options;
    groups_class *groups;
    properties_class *properties;
    submit *_submit;
    QTreeWidgetItem *first_job_item;

    QTreeWidget *tree;
    QVBoxLayout *layout;
    title_bar *_title_bar;

    void connections();

    // Jobs Funcions
    void setup_ui();
    void popup();
    void show_log();
    void delete_start(QString action);
    void to_action(QString action);

    void message(void (jobs_class::*funtion)(QString), QString action,
                 QString ask, QString tile, jobs_class *_class);

    void item_delete();

    void selection_changed();

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public:
    jobs_class(shared_variables *_shared, QWidget *_monitor, log_class *_log,
               servers_class *_servers, options_class *_options,
               groups_class *_groups, properties_class *_properties,
               submit *_submit);
    ~jobs_class();

    // Job Acciones
    QAction *delete_action;
    QAction *job_pause_action;
    QAction *job_resume_action;
    QAction *job_restart_action;
    QAction *job_unlock_servers_action;
    QAction *job_log_action;
    QAction *job_modify_action;

    inline QTreeWidget *get_tree() const;
};

inline QTreeWidget *jobs_class::get_tree() const
{
    return tree;
}

#endif // JOBS_HPP
