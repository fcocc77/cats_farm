#ifndef SUBMIT_H
#define SUBMIT_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>

#include "combo_box.h"
#include "ffmpeg_submit.h"
#include "time_knobs.h"

class submit : public QWidget
{
private:
    QWidget *_monitor;
    QVBoxLayout *layout;
    bool savePanel = false;

    combo_box *software_box;
    ffmpeg_submit *ffmpeg_widget;
    time_knobs *_time_knobs;

    QLineEdit *project_dir_edit;
    QPushButton *project_dir_button;
    QLabel *project_dir_label;

    QPushButton *project_button;
    QLineEdit *project_edit;
    QLabel *project_label;

    QLineEdit *render_node_edit;
    QLabel *render_node_label;

    QLineEdit *job_name;
    combo_box *server_group_box;
    combo_box *priority;
    QLineEdit *comment_edit;
    QCheckBox *suspend_box;
    QPushButton *submit_button;

    void ui();
    void connections();

    void submit_start(QString software);
    void send_job(QJsonArray info);
    void update_server_groups();
    void set_software(QString software);
    void panel_save();
    void panel_open();
    void submit_file(QString file);

    void update_ffmpeg_panel();
    void calc_ffmpeg_data(QString file, int *first_frame, int *last_frame,
                          int *task_size);

    void hideEvent(QHideEvent *event) override;

public:
    submit(QWidget *_monitor);
    ~submit();

    void submit_files(QStringList files);
};

#endif // SUBMIT_H
