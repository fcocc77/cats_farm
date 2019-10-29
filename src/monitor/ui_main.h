/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *jobs;
    QMenuBar *menu_bar;
    QMenu *menuhola;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *servers;
    QDockWidget *dockWidget1;
    QWidget *dockWidgetContents1;
    QVBoxLayout *verticalLayout_21;
    QTreeWidget *tasks;
    QDockWidget *dockWidget2;
    QWidget *dockWidgetContents2;
    QVBoxLayout *verticalLayout_22;
    QTreeWidget *groups;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setStyleSheet(QLatin1String("/* Black blue color */\n"
"QWidget {\n"
"  color: #c7cbd1;\n"
"  background-color: #1c1e21;\n"
"  border: 0px;\n"
"  font: 8pt \"Tahoma\"; }\n"
"\n"
"QMainWindow::separator {\n"
"  background-color: #17191c;\n"
"  width: 2px;\n"
"  border-style: outset;\n"
"  border-width: 1px; }\n"
"\n"
"QLabel {\n"
"  background-color: transparent; }\n"
"\n"
"QToolBar {\n"
"  padding: 10;\n"
"  border: 1px solid #101213;\n"
"  border-style: outset; }\n"
"  QToolBar:top, QToolBar:bottom {\n"
"    background-color: #1e2124; }\n"
"  QToolBar::separator {\n"
"    background-color: #2e3238;\n"
"    width: 1;\n"
"    margin: 7; }\n"
"  QToolBar QToolButton {\n"
"    background-color: transparent; }\n"
"    QToolBar QToolButton::hover {\n"
"      background-color: #2e3238;\n"
"      border: 1px solid #33373d; }\n"
"\n"
"QTabBar::tab {\n"
"  background-color: #2a2d32;\n"
"  padding: 4px;\n"
"  border: 1px solid #2e3238; }\n"
"\n"
"QMenuBar {\n"
"  background-color: #272b2f; }\n"
"  QMenuBar::item {\n"
"    background-color: transpar"
                        "ent; }\n"
"  QMenuBar::item:pressed {\n"
"    background-color: #2e3238;\n"
"    border: 1px solid #23262a; }\n"
"\n"
"QMenu {\n"
"  background-color: #23262a; }\n"
"  QMenu::item {\n"
"    padding: 2px 20px 2px 20px;\n"
"    border: 1px solid transparent;\n"
"    spacing: 20px; }\n"
"  QMenu::item:selected {\n"
"    background-color: #325064; }\n"
"  QMenu::separator {\n"
"    background-color: #2e3238;\n"
"    margin: 5px;\n"
"    height: 1px; }\n"
"  QMenu::indicator {\n"
"    width: 20px;\n"
"    height: 13px; }\n"
"\n"
"QTreeWidget {\n"
"  outline: 0;\n"
"  alternate-background-color: #2a2d32;\n"
"  background-color: #23262a;\n"
"  border: 1px solid #2a2d32; }\n"
"  QTreeWidget::branch {\n"
"    border-bottom: 1px solid #2e3238; }\n"
"    QTreeWidget::branch:has-children:closed {\n"
"      image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/right_arrow.png);\n"
"      background-color: transparent; }\n"
"      QTreeWidget::branch:has-children:closed:selected {\n"
"        background"
                        "-color: transparent;\n"
"        border-bottom: 1px solid #2e3238;\n"
"        color: #2e3238; }\n"
"      QTreeWidget::branch:has-children:closed:hover {\n"
"        background-color: red; }\n"
"    QTreeWidget::branch:open {\n"
"      image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/down_arrow.png);\n"
"      background-color: transparent; }\n"
"      QTreeWidget::branch:open:selected {\n"
"        background-color: red;\n"
"        border-bottom: 1px solid #2e3238;\n"
"        color: #2e3238; }\n"
"    QTreeWidget::branch:selected {\n"
"      background-color: #2e3238;\n"
"      border-bottom: 1px solid #2e3238;\n"
"      color: #2e3238; }\n"
"  QTreeWidget::item {\n"
"    border-bottom: 1px solid #2e3238; }\n"
"    QTreeWidget::item:selected {\n"
"      background-color: #325064;\n"
"      border-bottom: 1px solid #2e3238;\n"
"      color: #c7cbd1; }\n"
"    QTreeWidget::item:hover {\n"
"      background-color: #325064; }\n"
"\n"
"QHeaderView::section {\n"
"  background-color: #2a"
                        "2d32;\n"
"  padding-left: 5px;\n"
"  padding-top: 2px;\n"
"  padding-bottom: 2px;\n"
"  border: 0px solid transparent;\n"
"  margin-left: 1px;\n"
"  margin-top: 1px;\n"
"  margin-bottom: 1px; }\n"
"\n"
"QScrollBar:horizontal {\n"
"  background-color: #2a2d32;\n"
"  height: 10px; }\n"
"\n"
"QScrollBar:vertical {\n"
"  background-color: #2a2d32;\n"
"  width: 10px; }\n"
"\n"
"QScrollBar::handle:horizontal, QScrollBar::handle:vertical {\n"
"  background-color: #325064;\n"
"  min-height: 20px;\n"
"  border-radius: 2px;\n"
"  margin-left: 11;\n"
"  margin-right: 11; }\n"
"\n"
"QScrollBar::add-line:horizontal, QScrollBar::add-line:vertical {\n"
"  border-radius: 2px;\n"
"  background-color: #325064; }\n"
"\n"
"QScrollBar::sub-line:horizontal, QScrollBar::sub-line:vertical {\n"
"  border-radius: 2px;\n"
"  background-color: #325064; }\n"
"\n"
"QScrollBar::right-arrow:horizontal {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/theme/rc/right_arrow.png);\n"
"  width: 7px;\n"
"  height: "
                        "7px; }\n"
"\n"
"QScrollBar::left-arrow:horizontal {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/theme/rc/left_arrow.png);\n"
"  width: 7px;\n"
"  height: 7px; }\n"
"\n"
"QScrollBar::up-arrow:vertical {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/theme/rc/up_arrow.png);\n"
"  width: 7px;\n"
"  height: 7px; }\n"
"\n"
"QScrollBar::down-arrow:vertical {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/theme/rc/down_arrow.png);\n"
"  width: 7px;\n"
"  height: 7px; }\n"
"\n"
"QScrollBar::add-page:horizontal, QScrollBar::add-page:vertical {\n"
"  background: none; }\n"
"\n"
"QScrollBar::sub-page:horizontal, QScrollBar::sub-page:vertical {\n"
"  background: none; }\n"
"\n"
"QDockWidget:floatable {\n"
"  titlebar-close-icon: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/radio_checked.png);\n"
"  titlebar-normal-icon: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/radio_unc"
                        "hecked.png);\n"
"  border: 1px solid red; }\n"
"\n"
"QDockWidget::focus {\n"
"  background-color: transparent; }\n"
"\n"
"QDockWidget::close-button {\n"
"  border: 10px;\n"
"  padding: 2px; }\n"
"  QDockWidget::close-button:hover {\n"
"    image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/radio_checked_focus.png); }\n"
"\n"
"QDockWidget::float-button {\n"
"  border: 10px;\n"
"  padding: 2px; }\n"
"  QDockWidget::float-button:hover {\n"
"    image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/radio_unchecked_focus.png); }\n"
"\n"
"QDockWidget::title {\n"
"  background-color: #1c1e21;\n"
"  padding-top: 2px;\n"
"  text-align: center; }\n"
"\n"
"QLineEdit, QTextEdit {\n"
"  background-color: #3a3f45;\n"
"  border: 1px solid #464b53;\n"
"  padding-left: 2px;\n"
"  border-radius: 4px; }\n"
"  QLineEdit:focus, QTextEdit:focus {\n"
"    border: 1px solid #747e8b; }\n"
"  QLineEdit:disabled, QTextEdit:disabled {\n"
"    background-color: #2a2d32;\n"
"    border: 1px soli"
                        "d #2e3238; }\n"
"\n"
"QCheckBox {\n"
"  background-color: transparent; }\n"
"  QCheckBox::indicator:checked {\n"
"    image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_checked.png); }\n"
"    QCheckBox::indicator:checked:focus {\n"
"      image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_checked_focus.png); }\n"
"  QCheckBox::indicator:unchecked {\n"
"    image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_unchecked.png); }\n"
"    QCheckBox::indicator:unchecked:focus {\n"
"      image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_unchecked_focus.png); }\n"
"\n"
"QTreeView::indicator:unchecked {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_unchecked.png); }\n"
"\n"
"QTreeView::indicator:checked {\n"
"  image: url(/home/pancho/Documents/GitHub/cats_farm/src/monitor/sass/images/checkbox_checked.png); }\n"
"\n"
"QPushButton {\n"
"  backgro"
                        "und-color: #2e3238;\n"
"  border: 1px solid #3a3f45;\n"
"  border-radius: 2px; }\n"
"  QPushButton::hover {\n"
"    background-color: #464b53;\n"
"    border-color: #5d646f; }\n"
"\n"
"QFrame {\n"
"  border-radius: 3px; }\n"
"\n"
"QComboBox {\n"
"  background-color: #2e3238;\n"
"  border: 0px solid transparent;\n"
"  padding-left: 4px; }\n"
"  QComboBox:focus {\n"
"    border: 1px solid #33373d; }\n"
"  QComboBox::drop-down {\n"
"    background-color: transparent;\n"
"    width: 0px; }\n"
"  QComboBox:hover {\n"
"    background: #33373d; }\n"
"  QComboBox QAbstractItemView {\n"
"    background-color: #1e2124;\n"
"    border: 1px solid #23262a;\n"
"    border-radius: 0px; }\n"
"  QComboBox:item:selected {\n"
"    background-color: #325064;\n"
"    padding-left: 0px; }\n"
"  QComboBox::indicator {\n"
"    background-color: transparent; }\n"
"\n"
"QProgressBar:horizontal {\n"
"  text-align: center; }\n"
"  QProgressBar:horizontal::chunk:horizontal {\n"
"    background: #23262a;\n"
"    margin-top: 1px;\n"
"    ma"
                        "rgin-bottom: 0px;\n"
"    border-radius: 3px; }\n"
"\n"
"QWidget::disabled {\n"
"  color: #747e8b; }\n"
"\n"
"QWidget #style2 {\n"
"  background-color: transparent; }\n"
"\n"
"QWidget #style1 {\n"
"  background-color: #23262a;\n"
"  border: 1px solid #33373d;\n"
"  border-radius: 3px; }\n"
"\n"
"QWidget #QCodeEditorLineWidget {\n"
"  background-color: #1c1e21; }\n"
"\n"
"QWidget #Logs {\n"
"  background-color: #23262a;\n"
"  border-color: #ffaa00;\n"
"  border: 1px solid #2e3238;\n"
"  border-radius: 3px; }\n"
"\n"
"* {\n"
"  color: #c7cbd1;\n"
"  margin: 0;\n"
"  padding: 0;\n"
"  outline: 0;\n"
"  background: white;\n"
"  background-color: white;\n"
"  border: 0px;\n"
"  font: 8pt \"Tahoma\"; }\n"
"\n"
"#jobs {\n"
"  background-color: #23262a;\n"
"  border: 1px solid red; }\n"
"  #jobs QHeaderView::section {\n"
"    border: 1px solid red;\n"
"    padding: 5px 10px 5px 10px;\n"
"    background-color: #2a2d32;\n"
"    border: 0px solid transparent; }\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        jobs = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(8, QStringLiteral("Comment"));
        __qtreewidgetitem->setText(7, QStringLiteral("Total Render Time"));
        __qtreewidgetitem->setText(6, QStringLiteral("Finished Data/Time"));
        __qtreewidgetitem->setText(5, QStringLiteral("Submit Data/Time"));
        __qtreewidgetitem->setText(4, QStringLiteral("Status"));
        __qtreewidgetitem->setText(3, QStringLiteral("Progress"));
        __qtreewidgetitem->setText(2, QStringLiteral("Software"));
        __qtreewidgetitem->setText(1, QStringLiteral("Priority"));
        __qtreewidgetitem->setText(0, QStringLiteral("Job Name"));
        jobs->setHeaderItem(__qtreewidgetitem);
        jobs->setObjectName(QStringLiteral("jobs"));
        jobs->setIndentation(0);

        verticalLayout->addWidget(jobs);

        MainWindow->setCentralWidget(centralwidget);
        menu_bar = new QMenuBar(MainWindow);
        menu_bar->setObjectName(QStringLiteral("menu_bar"));
        menu_bar->setGeometry(QRect(0, 0, 766, 22));
        menuhola = new QMenu(menu_bar);
        menuhola->setObjectName(QStringLiteral("menuhola"));
        MainWindow->setMenuBar(menu_bar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        servers = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(8, QStringLiteral("Job Rendered"));
        __qtreewidgetitem1->setText(7, QStringLiteral("VM"));
        __qtreewidgetitem1->setText(6, QStringLiteral("IP"));
        __qtreewidgetitem1->setText(5, QStringLiteral("System"));
        __qtreewidgetitem1->setText(4, QStringLiteral("RAM Usage"));
        __qtreewidgetitem1->setText(3, QStringLiteral("CPU Usage"));
        __qtreewidgetitem1->setText(2, QStringLiteral("Ins."));
        __qtreewidgetitem1->setText(1, QStringLiteral("Status"));
        __qtreewidgetitem1->setText(0, QStringLiteral("Server Name"));
        servers->setHeaderItem(__qtreewidgetitem1);
        servers->setObjectName(QStringLiteral("servers"));

        verticalLayout_2->addWidget(servers);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget1 = new QDockWidget(MainWindow);
        dockWidget1->setObjectName(QStringLiteral("dockWidget1"));
        dockWidgetContents1 = new QWidget();
        dockWidgetContents1->setObjectName(QStringLiteral("dockWidgetContents1"));
        verticalLayout_21 = new QVBoxLayout(dockWidgetContents1);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        tasks = new QTreeWidget(dockWidgetContents1);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setText(4, QStringLiteral("Time"));
        __qtreewidgetitem2->setText(3, QStringLiteral("Server"));
        __qtreewidgetitem2->setText(2, QStringLiteral("Status"));
        __qtreewidgetitem2->setText(1, QStringLiteral("Frame Range"));
        __qtreewidgetitem2->setText(0, QStringLiteral("Task"));
        tasks->setHeaderItem(__qtreewidgetitem2);
        tasks->setObjectName(QStringLiteral("tasks"));

        verticalLayout_21->addWidget(tasks);

        dockWidget1->setWidget(dockWidgetContents1);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget1);
        dockWidget2 = new QDockWidget(MainWindow);
        dockWidget2->setObjectName(QStringLiteral("dockWidget2"));
        dockWidgetContents2 = new QWidget();
        dockWidgetContents2->setObjectName(QStringLiteral("dockWidgetContents2"));
        verticalLayout_22 = new QVBoxLayout(dockWidgetContents2);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        groups = new QTreeWidget(dockWidgetContents2);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem();
        __qtreewidgetitem3->setText(1, QStringLiteral("Status"));
        __qtreewidgetitem3->setText(0, QStringLiteral("Group Name"));
        groups->setHeaderItem(__qtreewidgetitem3);
        groups->setObjectName(QStringLiteral("groups"));

        verticalLayout_22->addWidget(groups);

        dockWidget2->setWidget(dockWidgetContents2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget2);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menu_bar->addAction(menuhola->menuAction());
        menuhola->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CatsFarm Monitor", Q_NULLPTR));
        menuhola->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Servers", Q_NULLPTR));
        dockWidget1->setWindowTitle(QApplication::translate("MainWindow", "Tasks", Q_NULLPTR));
        dockWidget2->setWindowTitle(QApplication::translate("MainWindow", "Groups", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
