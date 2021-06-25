#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "combo_box.h"
#include "submit.h"

submit::submit()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    // New Widgets
    QWidget *project_dir_widget = new QWidget();
    QHBoxLayout *project_dir_layout = new QHBoxLayout(project_dir_widget);
    QLineEdit *project_dir_edit = new QLineEdit();
    QPushButton *project_dir_button = new QPushButton("Open Dir Project");

    QWidget *project_path_widget = new QWidget();
    QHBoxLayout *project_path_layout = new QHBoxLayout(project_path_widget);
    QLineEdit *project_path_edit = new QLineEdit();
    QPushButton *project_path_button = new QPushButton("Open Project");

    QWidget *frame_range_widget = new QWidget();
    QHBoxLayout *frame_range_layout = new QHBoxLayout(frame_range_widget);
    QLabel *frame_range_label = new QLabel("Frame Range: ");
    QLineEdit *first_frame_edit = new QLineEdit();
    QLineEdit *last_frame_edit = new QLineEdit();

    QWidget *task_size_widget = new QWidget();
    QHBoxLayout *task_size_layout = new QHBoxLayout(task_size_widget);
    QLabel *task_size_label = new QLabel("Task Size: ");
    QLineEdit *task_size_edit = new QLineEdit();

    QWidget *priority_widget = new QWidget();
    QHBoxLayout *priority_layout = new QHBoxLayout(priority_widget);
    QLabel *priority_label = new QLabel("Priority: ");
    combo_box *priority_box = new combo_box();

    QWidget *comment_widget = new QWidget();
    QHBoxLayout *comment_layout = new QHBoxLayout(comment_widget);
    QLabel *comment_label = new QLabel("Comment: ");
    QLineEdit *comment_edit = new QLineEdit();

    QWidget *dialog_widget = new QWidget();
    QHBoxLayout *dialog_layout = new QHBoxLayout(dialog_widget);
    QPushButton *cancel_button = new QPushButton("Cancel");
    QPushButton *submit_button = new QPushButton("Submit");

    // Widgets Properties
    first_frame_edit->setPlaceholderText("First Frame");
    last_frame_edit->setPlaceholderText("Last Frame");

    priority_box->add_items(
        {"Very Slow", "Slow", "Normal", "High", "Very High"});

    // Layout
    project_path_layout->addWidget(project_path_edit);
    project_path_layout->addWidget(project_path_button);

    project_dir_layout->addWidget(project_dir_edit);
    project_dir_layout->addWidget(project_dir_button);

    frame_range_layout->addWidget(frame_range_label);
    frame_range_layout->addWidget(first_frame_edit);
    frame_range_layout->addWidget(last_frame_edit);

    task_size_layout->addWidget(task_size_label);
    task_size_layout->addWidget(task_size_edit);

    priority_layout->addWidget(priority_label);
    priority_layout->addWidget(priority_box);

    comment_layout->addWidget(comment_label);
    comment_layout->addWidget(comment_edit);

    dialog_layout->addStretch();
    dialog_layout->addWidget(cancel_button);
    dialog_layout->addWidget(submit_button);

    layout->addWidget(project_dir_widget);
    layout->addWidget(project_path_widget);
    layout->addWidget(frame_range_widget);
    layout->addWidget(task_size_widget);
    layout->addWidget(priority_widget);
    layout->addWidget(comment_widget);
    layout->addWidget(dialog_widget);
}

submit::~submit() {}
