#ifndef SETTINGS_MONITOR_H
#define SETTINGS_MONITOR_H

#include <QWidget>
#include <QJsonArray>
#include <QTreeWidget>

#include "text_knob.h"
#include "shared_variables.h"
#include "combo_box.h"

class settings_monitor : public QWidget
{
private:
    shared_variables *shared;
    QTreeWidget *managers_tree;
    QLineEdit *maker_name_edit;

    combo_box *submit_ip;
    QString current_submit_ip;

    void add_manager();
    void update_submit_ip();

public:
    settings_monitor(shared_variables *shared);
    void save();
    void restore();

    QJsonArray get_hosts() const;
};
#endif // SETTINGS_MONITOR_H
