#include <QTabWidget>
#include <QVBoxLayout>

#include "settings_manager.h"
#include "../global/global.h"
#include "tcp.h"

settings_manager::settings_manager(shared_variables *_shared)
    : shared(_shared)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QTabWidget *settings_tab = new QTabWidget();

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

    // Layout
    layout->addWidget(settings_tab);
}

void settings_manager::save()
{
    path_write();

}

void settings_manager::update(QString host)
{
    shared->manager_host = host;
    path_read();
}


void settings_manager::path_read()
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
    }
}

void settings_manager::path_write()
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
