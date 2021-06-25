#include <QVBoxLayout>

#include "util.h"
#include "tcp.h"
#include "settings.h"
#include "../global/global.h"

settings_class::settings_class(shared_variables *_shared)
    : shared(_shared)
{
    setup_ui();
    connections();
}

settings_class::~settings_class() {}

void settings_class::setup_ui()
{
    this->setObjectName("settings");
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setMargin(0);

    {
        QLayout *zones_knobs = qlayout(main_layout, "h", "zones_knobs");
        {
            QLabel *zones_label = new QLabel("Manager ZONES:");
            zones_knobs->addWidget(zones_label);

            zones_edit = new QLineEdit();
            zones_knobs->addWidget(zones_edit);
        }
        QLayout *host_knobs = qlayout(main_layout, "h", "host_knobs");
        {
            QLabel *host_label = new QLabel("Manager HOST:");
            host_knobs->addWidget(host_label);

            host_edit = new QLineEdit();
            host_knobs->addWidget(host_edit);
        }

        QLayout *third_knobs = qlayout(main_layout, "v", "third_knobs");
        {
            QTabWidget *settings_tab = new QTabWidget();
            third_knobs->addWidget(settings_tab);
            third_knobs->setMargin(0);
            {
                auto addTab = [=](QString name, QPlainTextEdit *text) {
                    QWidget *tab = new QWidget();
                    QVBoxLayout *tab_layout = new QVBoxLayout();
                    tab->setLayout(tab_layout);
                    tab_layout->addWidget(text);
                    settings_tab->addTab(tab, name);
                };

                paths_text = new QPlainTextEdit();
                addTab("Paths", paths_text);

                maya_text = new QPlainTextEdit();
                addTab("Maya", maya_text);

                nuke_text = new QPlainTextEdit();
                addTab("Nuke", nuke_text);

                houdini_text = new QPlainTextEdit();
                addTab("Houdini", houdini_text);

                ntp_text = new QPlainTextEdit();
                addTab("Natron Ntp", ntp_text);

                natron_text = new QPlainTextEdit();
                addTab("Natron", natron_text);

                ae_text = new QPlainTextEdit();
                addTab("After Effect", ae_text);
            }
        }

        QLayout *fourth_knobs = qlayout(main_layout, "v");
        {
            QLayout *button_knobs = qlayout(fourth_knobs, "h");
            fourth_knobs->setMargin(0);
            button_knobs->setMargin(0);
            {
                cancel_button = new QPushButton("Cancel");
                button_knobs->addWidget(cancel_button);

                apply_button = new QPushButton("Apply");
                button_knobs->addWidget(apply_button);

                ok_button = new QPushButton("OK");
                button_knobs->addWidget(ok_button);
            }
        }
    }
}

void settings_class::connections()
{
    connect(cancel_button, &QPushButton::clicked, this,
            [this]() { this->parentWidget()->parentWidget()->hide(); });
    connect(apply_button, &QPushButton::clicked, this,
            [this]() { this->ok(); });
    connect(ok_button, &QPushButton::clicked, this, [this]() {
        this->ok();
        this->parentWidget()->parentWidget()->hide();
    });
}

void settings_class::update(QString host)
{
    shared->manager_host = host;
    path_read();
}

void settings_class::ok()
{
    path_write();

    // guarda host del manager
    QJsonObject manager = shared->settings["manager"].toObject();
    manager["ip"] = host_edit->text();
    shared->settings["manager"] = manager;
    // ---------------------------

    // guarda lista de ips de QString a un json, y borra los espacios
    QString zones = zones_edit->text();
    QJsonArray json_hosts;
    for (QString zone : zones.split(","))
        json_hosts.push_back(zone.replace(" ", ""));

    shared->settings["hosts"] = json_hosts;
    // -----------------------------
    jwrite(VINARENDER_PATH + "/etc/settings.json", shared->settings);

    // agrega las ips al combobox de zonas
    shared->zone_box->clear();
    for (QJsonValue ip : json_hosts)
        shared->zone_box->addItem(ip.toString());
    // -------------------------
}

void settings_class::path_read()
{
    QJsonArray send = {"preferences", QJsonArray({"read", "none"})};
    QString recv =
        tcpClient(shared->manager_host, shared->manager_port, jats({3, send}));
    QJsonObject preferences = jofs(recv);

    auto array_to_string = [](QJsonArray array) {
        QString ret;
        for (QJsonValue value : array)
            ret += value.toString() + "\n";
        return ret.left(ret.length() - 1); // borra la ultima linea
    };

    if (not preferences.empty())
    {
        QJsonObject paths = preferences["paths"].toObject();
        QString system, nuke, maya, houdini, ntp, natron, ae;

        system = array_to_string(paths["system"].toArray());
        nuke = array_to_string(paths["nuke"].toArray());
        houdini = array_to_string(paths["houdini"].toArray());
        ntp = array_to_string(paths["ntp"].toArray());
        natron = array_to_string(paths["natron"].toArray());
        maya = array_to_string(paths["maya"].toArray());
        ae = array_to_string(paths["ae"].toArray());

        paths_text->setPlainText(system);
        nuke_text->setPlainText(nuke);
        maya_text->setPlainText(maya);
        houdini_text->setPlainText(houdini);
        ntp_text->setPlainText(ntp);
        natron_text->setPlainText(natron);
        ae_text->setPlainText(ae);

        // setea los hosts guardados
        QString hosts;
        for (QJsonValue host : shared->settings["hosts"].toArray())
            hosts += host.toString() + ", ";
        hosts = hosts.left(hosts.length() - 2);

        zones_edit->setText(hosts);
        // ------------------------------

        // setea el host del manager
        QString host = shared->settings["manager"].toObject()["ip"].toString();
        host_edit->setText(host);
        // ------------------
    }
}

void settings_class::path_write()
{

    QJsonObject paths;

    QJsonArray system;
    for (auto l : paths_text->toPlainText().split("\n"))
        system.push_back(l);
    paths["system"] = system;

    QJsonArray nuke;
    for (auto l : nuke_text->toPlainText().split("\n"))
        nuke.push_back(l);
    paths["nuke"] = nuke;

    QJsonArray maya;
    for (auto l : maya_text->toPlainText().split("\n"))
        maya.push_back(l);
    paths["maya"] = maya;

    QJsonArray houdini;
    for (auto l : houdini_text->toPlainText().split("\n"))
        houdini.push_back(l);
    paths["houdini"] = houdini;

    QJsonArray ntp;
    for (auto l : ntp_text->toPlainText().split("\n"))
        ntp.push_back(l);
    paths["ntp"] = ntp;

    QJsonArray natron;
    for (auto l : natron_text->toPlainText().split("\n"))
        natron.push_back(l);
    paths["natron"] = natron;

    QJsonArray ae;
    for (auto l : ae_text->toPlainText().split("\n"))
        ae.push_back(l);

    paths["ae"] = ae;

    tcpClient(shared->manager_host, shared->manager_port,
              jats({3, {{"preferences", {{"write", paths}}}}}));
}
