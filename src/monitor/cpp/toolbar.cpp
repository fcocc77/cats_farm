#include "../hpp/toolbar.hpp"

toolbar_class::toolbar_class(
    Ui::MainWindow *_ui,
    global_class *_global)
{
    ui = _ui;
    global = _global;
    property();
}

toolbar_class::~toolbar_class()
{
}

void toolbar_class::property()
{

    ui->toolbar->setMovable(0);
    ui->toolbar->setIconSize(QSize(30, 30));

    // Acciones
	// ui->toolbar->addAction(jobActions->jobResumeAction);
	// ui->toolbar->addSeparator();
	// ui->toolbar->addAction(jobActions->jobSuspendAction);
	// ui->toolbar->addSeparator();
	ui->toolbar->addAction(global->preferences_action);
	ui->toolbar->addSeparator();
	// ui->toolbar->addAction(_general->panelSubmitAction);
	// ui->toolbar->addSeparator();
	// ui->toolbar->addWidget(tmpButton);
    // ------------------

    /*
	QPushButton *tmpButton = new QPushButton();

	connect(tmpButton, &QPushButton::clicked, this, [this]() {
		_general->style_ui();
	});

	// toolBar

	
	
	toolBar->setWindowTitle("Tools Bar");
	//---------------------
    */
}