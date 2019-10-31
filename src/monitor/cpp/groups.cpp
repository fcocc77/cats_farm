#include "../hpp/groups.hpp"

groups_class::groups_class(
    Ui::MainWindow *_ui,
    QMainWindow *_monitor,
    shared_variables *_shared)
{
    ui = _ui;
    monitor = _monitor;
    shared = _shared;
    // Group Action
    create_action = new QAction("Create Group");
    add_machine_action = new QAction("Add Machine");
    delete_action = new QAction("Delete Group");
    //------------------------------------------------
    managerHost = fread(path + "/etc/manager_host");
    properties();
    connections();
}

groups_class::~groups_class()
{
}

void groups_class::properties()
{
    ui->groups->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion

    ui->groups->setColumnHidden(2, true);

    // ajusta el largo de las columnas
    ui->groups->setColumnWidth(0, 150);
    ui->groups->setColumnWidth(1, 70);
    ui->groups->setColumnWidth(2, 70);
    //----------------------------------

    ui->groups->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->groups->setSortingEnabled(true);
}

void groups_class::connections()
{

    connect(ui->groups, &QTreeWidget::customContextMenuRequested, this, &groups_class::popup);

    // Group Action
    connect(create_action, &QAction::triggered, this, &groups_class::create_window);
    create_action->setShortcut(QString("Ctrl+G"));
    connect(add_machine_action, &QAction::triggered, this, &groups_class::add_machine);

    connect(delete_action, &QAction::triggered, this, &groups_class::group_delete);

    //--------------------------------------------------------------------
}

void groups_class::popup()
{

    auto selected = ui->groups->selectedItems();

    QMenu *menu = new QMenu(monitor);
    if (not selected.empty())
    {

        menu->addAction(add_machine_action);
        menu->addSeparator();
        menu->addAction(delete_action);
        menu->popup(QCursor::pos());
    }
    else
    {
        menu->addAction(create_action);
        menu->popup(QCursor::pos());
    }
}

void groups_class::create_window()
{

    // funcion anonima de grupo
    auto groupCreate = [this](QString group_name) {
        qDebug() << group_name;
        shared->stopUpdate = true;
        auto selected = ui->servers->selectedItems();

        QJsonArray machines_send;
        QJsonArray machines;

        for (auto item : selected)
        {
            machines_send.push_back(item->text(0));
            machines.push_back(QJsonArray({item->text(0), true}));
        }

        QJsonArray group = {group_name, machines_send.size(), 0, machines_send};
        QJsonArray pks = {"groupCreate", group};

        tcpClient(managerHost, 7000, jats({3, pks}));

        QTreeWidgetItem *item = group_make(group_name, machines.size(), 0, 0);

        make_server(item, machines);
        //submitUpdateBox();
    };
    //-----------------------------------------------

    bool ok;
    QString group_name = QInputDialog::getText(monitor, "Server Group", "Enter Group name:",
                                               QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok)
    {
        if (not group_name.isEmpty())
        {
            groupCreate(group_name);
        }
        else
        {
            groupCreate("Nameless");
        }
    }
}

QTreeWidgetItem *groups_class::group_make(
    QString group_name,
    int totaMachine,
    int activeMachine,
    int offMachine)
{

    // Name Column
    QVBoxLayout *name_layout = new QVBoxLayout();
    name_layout->setSpacing(0);
    name_layout->setContentsMargins(7, 3, 0, 3);

    ElidedLabel *name_label = new ElidedLabel();
    name_label->setText(group_name);
    name_label->setStyleSheet("QLabel{color: rgb(220,220,220);  font:10pt;}");
    name_layout->addWidget(name_label);

    ElidedLabel *subName_label = new ElidedLabel();
    subName_label->setText("Group is being used");
    subName_label->setStyleSheet("QLabel{color: rgb(100,150,220);  font:8pt;}");
    name_layout->addWidget(subName_label);

    QWidget *name_widget = new QWidget();
    name_widget->setStyleSheet("QWidget{background-color: rgba(0,0,0,0);}");
    name_widget->setLayout(name_layout);
    //-----------------------------------------------------------------------

    // On Machines
    ElidedLabel *on_label = new ElidedLabel();
    on_label->setText("ON Machine:  ");

    ElidedLabel *on_label_value = new ElidedLabel();
    on_label_value->setText(QString::number(activeMachine));

    QHBoxLayout *on_layaout = new QHBoxLayout();
    on_layaout->setContentsMargins(0, 0, 0, 0);
    on_layaout->setSpacing(0);

    on_layaout->addWidget(on_label);
    on_layaout->addWidget(on_label_value);

    QWidget *on_widget = new QWidget();
    on_widget->setLayout(on_layaout);
    //------------------------------------------------

    // Off Machines
    ElidedLabel *off_label = new ElidedLabel();
    off_label->setText("OFF Machine: ");

    ElidedLabel *off_label_value = new ElidedLabel();
    off_label_value->setText(QString::number(offMachine));

    QHBoxLayout *off_layaout = new QHBoxLayout();
    off_layaout->setContentsMargins(0, 0, 0, 0);
    off_layaout->setSpacing(0);

    off_layaout->addWidget(off_label);
    off_layaout->addWidget(off_label_value);

    QWidget *off_widget = new QWidget();
    off_widget->setLayout(off_layaout);
    //-----------------------------------------------

    // All Machines
    ElidedLabel *all_label = new ElidedLabel();
    all_label->setText("All Machine: ");

    ElidedLabel *all_label_value = new ElidedLabel();
    all_label_value->setText(QString::number(totaMachine));

    QHBoxLayout *all_layaout = new QHBoxLayout();
    all_layaout->setContentsMargins(0, 0, 0, 0);
    all_layaout->setSpacing(0);

    all_layaout->addWidget(all_label);
    all_layaout->addWidget(all_label_value);

    QWidget *all_widget = new QWidget();
    all_widget->setLayout(all_layaout);
    //-----------------------------------------------

    // Union de on y off widget
    QVBoxLayout *on_off_layout = new QVBoxLayout();
    on_off_layout->setContentsMargins(0, 0, 0, 0);
    on_off_layout->setSpacing(0);
    on_off_layout->setContentsMargins(7, 3, 0, 3);

    on_off_layout->addWidget(on_widget);
    on_off_layout->addWidget(off_widget);

    QWidget *on_off_widget = new QWidget();
    on_off_widget->setLayout(on_off_layout);
    //------------------------------------------

    // Status Columm
    QHBoxLayout *status_layout = new QHBoxLayout();
    status_layout->setContentsMargins(0, 0, 0, 0);
    status_layout->setSpacing(0);
    status_layout->setContentsMargins(7, 3, 0, 3);

    status_layout->addWidget(on_off_widget);
    status_layout->addWidget(all_widget);

    QWidget *status_widget = new QWidget();
    status_widget->setLayout(status_layout);
    status_widget->setStyleSheet("QWidget{background-color: rgba(0,0,0,0);}");
    //--------------------------------------------------------------------------

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(2, group_name);

    ui->groups->addTopLevelItem(item);
    ui->groups->setItemWidget(item, 0, name_widget);
    ui->groups->setItemWidget(item, 1, status_widget);

    return item;
}

void groups_class::make_server(
    QTreeWidgetItem *item,
    QJsonArray machines)
{
    struct _name_item
    {
        QString name;
        QTreeWidgetItem *item;
        QString status;
    };

    // crea lista de los childItem antiguos
    QList<_name_item> oldChild;
    QStringList oldChildName;

    for (int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem *childItem = item->child(i);
        QString childName = childItem->text(0);
        oldChild.push_back({childName, childItem, NULL});
        oldChildName.push_back(childName);
    }
    //-------------------------------------------------

    // crea lista de los nuevos childItem
    QList<_name_item> newChild;
    QStringList newChildName;

    for (QJsonValue server : machines)
    {
        QString server_name = server.toArray()[0].toString();
        QString status = "Off";
        if (server.toArray()[1].toBool())
            status = "On";

        QTreeWidgetItem *childItem = new QTreeWidgetItem();
        newChild.push_back({server_name, childItem, status});
        newChildName.push_back(server_name);
    }
    //-------------------------------------------

    // si los nuevos childItem no estan en el antiguo, los crea
    for (auto child : newChild)
    {
        if (not oldChildName.contains(child.name))
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, child.name);
            childItem->setText(1, child.status);
            item->addChild(childItem);
        }
    }
    //-------------------------------------------------------

    // si los antiguos childItem no estan en los nuevos, los borra
    for (auto child : oldChild)
    {
        if (not newChildName.contains(child.name))
        {
            auto root = ui->groups->invisibleRootItem();
            root->removeChild(child.item);
            child.item->parent()->removeChild(child.item);
        }
    }
    //---------------------------------------------------------

    //Actualiza el status del childItem
    for (int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem *childItem = item->child(i);

        for (auto child : newChild)
        {
            if (child.name == childItem->text(0))
            {
                childItem->setText(1, child.status);

                if (child.status == "Off")
                {
                    childItem->setForeground(0, QColor(255, 30, 30));
                    childItem->setForeground(1, QColor(255, 30, 30));
                }
                else
                {
                    childItem->setForeground(0, QColor(200, 200, 200));
                    childItem->setForeground(1, QColor(100, 200, 100));
                }
            }
        }
    }

    //---------------------------------------------
}

void groups_class::add_machine()
{

    shared->stopUpdate = true;

    // Obtiene server seleccionados
    QJsonArray server_list;
    for (auto item : ui->servers->selectedItems())
    {
        QString server_name = item->text(0);
        server_list.push_back(server_name);
    }

    //-----------------------------------------------

    QJsonArray group_machine;
    for (auto item : ui->groups->selectedItems())
    {
        QString group_name = item->text(2);

        // crea lista de los childItem antiguos
        QStringList oldChild;
        for (int i = 0; i < item->childCount(); ++i)
        {
            auto childItem = item->child(i);
            QString childName = childItem->text(0);
            oldChild.push_back(childName);
        }
        //-------------------------------------------------

        for (QJsonValue serverName : server_list)
        {
            if (not oldChild.contains(serverName.toString()))
            {
                QTreeWidgetItem *childItem = new QTreeWidgetItem();

                childItem->setText(0, serverName.toString());
                childItem->setText(1, "On");
                childItem->setForeground(1, QColor(100, 200, 100));

                item->addChild(childItem);
            }
        }
        group_machine.push_back({{group_name, server_list}});
    }

    QJsonArray groups = {"group_list", group_machine, "addMachine"};
    QJsonArray pks = {"groupAction", groups};

    tcpClient(managerHost, 7000, jats({3, pks}));
}

void groups_class::group_delete()
{

    auto selected = ui->groups->selectedItems();
    if (not selected.empty())
    {

        QString tile = "Group Delete";
        QString ask = "Sure you want to delete the group?";

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(monitor, tile, ask, QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {

            shared->stopUpdate = true;

            auto root = ui->groups->invisibleRootItem();

            QJsonArray group_machine, group_list;

            for (auto item : ui->groups->selectedItems())
            {

                if (item->parent())
                {
                    QString group_name = item->parent()->text(2);
                    QString server_name = item->text(0);

                    group_machine.push_back({{group_name, server_name}});

                    item->parent()->removeChild(item);
                }
                else
                {
                    QString group_name = item->text(2);
                    group_list.push_back(group_name);

                    root->removeChild(item);
                }
            }

            QJsonArray groups = {group_list, group_machine, "delete"};

            QJsonArray pks = {"groupAction", groups};
            tcpClient(managerHost, 7000, jats({3, pks}));
        }
    }
}
