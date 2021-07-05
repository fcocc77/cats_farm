#include <QVBoxLayout>

#include "../global/global.h"
#include "main_window.h"
#include "toolbar.h"
#include "settings.h"
#include "submit_global.h"
#include "tcp.h"
#include "util.h"

settings_class::settings_class(QWidget *__monitor, shared_variables *_shared)
    : _monitor(__monitor)
    , shared(_shared)
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

    QWidget *hosts_widget = new QWidget;
    hosts_widget->setObjectName("hosts_widget");
    QVBoxLayout *hosts_layout = new QVBoxLayout(hosts_widget);
    hosts_layout->setMargin(0);
    hosts_layout->setSpacing(0);

    zones_edit = new text_knob("Manager ZONES");
    host_edit = new text_knob("Manager HOST");

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    zones_edit->layout()->setContentsMargins(h, v, h, s);
    host_edit->layout()->setContentsMargins(h, s, h, v);

    QWidget *tabs_widget = new QWidget;
    tabs_widget->setObjectName("tabs_widget");
    QVBoxLayout *tabs_layout = new QVBoxLayout(tabs_widget);

    QTabWidget *settings_tab = new QTabWidget();

    tabs_layout->setMargin(0);

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

    ffmpeg_text = new QPlainTextEdit();
    addTab("FFmpeg", ffmpeg_text);

    vinacomp_text = new QPlainTextEdit();
    addTab("VinaComp", vinacomp_text);

    QWidget *buttons_widget = new QWidget;
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_widget);

    buttons_layout->setMargin(0);

    cancel_button = new QPushButton("Cancel");
    apply_button = new QPushButton("Apply");
    ok_button = new QPushButton("OK");

    // Layout
    hosts_layout->addWidget(zones_edit);
    hosts_layout->addWidget(host_edit);

    tabs_layout->addWidget(settings_tab);

    buttons_layout->addWidget(cancel_button);
    buttons_layout->addWidget(apply_button);
    buttons_layout->addWidget(ok_button);

    main_layout->addWidget(hosts_widget);
    main_layout->addWidget(tabs_widget);
    main_layout->addWidget(buttons_widget);
}

void settings_class::connections()
{
    connect(cancel_button, &QPushButton::clicked, this, [this]() {
        static_cast<monitor *>(_monitor)->get_toolbar()->hide_properties();
    });

    connect(apply_button, &QPushButton::clicked, this,
            [this]() { this->ok(); });

    connect(ok_button, &QPushButton::clicked, this, [this]() {
        this->ok();
        static_cast<monitor *>(_monitor)->get_toolbar()->hide_properties();
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
    manager["ip"] = host_edit->get_text();
    shared->settings["manager"] = manager;

    // guarda lista de ips de QString a un json, y borra los espacios
    QString zones = zones_edit->get_text();
    QJsonArray json_hosts;
    for (QString zone : zones.split(","))
        json_hosts.push_back(zone.replace(" ", ""));

    shared->settings["hosts"] = json_hosts;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);

    // agrega las ips al combobox de zonas
    shared->zone_box->clear();
    for (QJsonValue ip : json_hosts)
        shared->zone_box->add_item(ip.toString());
}

void settings_class::path_read()
{
    QJsonArray send = {"preferences", QJsonArray({"read", "none"})};
    QString recv =
        tcpClient(shared->manager_host, MANAGER_PORT, jats({3, send}));
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
        QString system, nuke, maya, houdini, ffmpeg, vinacomp;

        system = array_to_string(paths["system"].toArray());
        nuke = array_to_string(paths["nuke"].toArray());
        houdini = array_to_string(paths["houdini"].toArray());
        maya = array_to_string(paths["maya"].toArray());
        ffmpeg = array_to_string(paths["ffmpeg"].toArray());
        vinacomp = array_to_string(paths["vinacomp"].toArray());

        paths_text->setPlainText(system);
        nuke_text->setPlainText(nuke);
        maya_text->setPlainText(maya);
        houdini_text->setPlainText(houdini);
        ffmpeg_text->setPlainText(ffmpeg);
        vinacomp_text->setPlainText(vinacomp);

        // setea los hosts guardados
        QString hosts;
        for (QJsonValue host : shared->settings["hosts"].toArray())
            hosts += host.toString() + ", ";
        hosts = hosts.left(hosts.length() - 2);

        zones_edit->set_text(hosts);

        // setea el host del manager
        QString host = shared->settings["manager"].toObject()["ip"].toString();
        host_edit->set_text(host);
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

    QJsonArray ffmpeg;
    for (auto l : ffmpeg_text->toPlainText().split("\n"))
        ffmpeg.push_back(l);
    paths["ffmpeg"] = ffmpeg;

    QJsonArray vinacomp;
    for (auto l : vinacomp_text->toPlainText().split("\n"))
        vinacomp.push_back(l);
    paths["vinacomp"] = vinacomp;

    tcpClient(shared->manager_host, MANAGER_PORT,
              jats({3, {{"preferences", {{"write", paths}}}}}));
}
