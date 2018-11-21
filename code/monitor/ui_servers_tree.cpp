#include "ui_servers_tree.h"

void ui_servers_tree::actions(){

	// display job list
	//shared->server_display=jread("../../etc/shared->server_display.json")
	//if not shared->server_display:

	shared->server_display = { { "mac", true }, { "linux", true }, { "window", true }, { "on", true }, 
								 { "off", true }, { "show", true }, { "hide", true } };

	auto displayAction = [this] ( QString action ){		
		if ( shared->server_display[ action ].toBool() ){
			shared->server_display[ action ] = false;
		}
		else{ shared->server_display[ action ] = true;
		}
	};

	auto displayAll = [this] ( bool status ){
		shared->server_display["mac"] = status;
		shared->server_display["linux"] = status;
		shared->server_display["window"] = status;
		shared->server_display["on"] = status;
		shared->server_display["off"] = status;
		shared->server_display["show"] = status;
		shared->server_display["hide"] = status;

		displayWindowsAction->setChecked(status);
		displayLinuxAction->setChecked(status);
		displayMacAction->setChecked(status);
		displayOnAction->setChecked(status);
		displayOffAction->setChecked(status);
	};

	connect( showAllAction, &QAction::triggered, this, [=] (){ displayAll( true ); } );
	connect( hideAllAction, &QAction::triggered, this, [=] (){ displayAll( false ); } );

	connect( displayWindowsAction, &QAction::triggered, this, [=] (){ displayAction("window"); } );
	displayWindowsAction->setCheckable(true);
	displayWindowsAction->setChecked( shared->server_display["window"].toBool() );

	connect( displayLinuxAction, &QAction::triggered, this, [=] (){ displayAction("linux"); } );
	displayLinuxAction->setCheckable(true);
	displayLinuxAction->setChecked( shared->server_display["linux"].toBool() );

	connect( displayMacAction, &QAction::triggered, this, [=] (){ displayAction("mac"); } );
	displayMacAction->setCheckable(true);
	displayMacAction->setChecked( shared->server_display["mac"].toBool() );

	connect( displayOnAction, &QAction::triggered, this, [=] (){ displayAction("on"); } );
	displayOnAction->setCheckable(true);
	displayOnAction->setChecked( shared->server_display["on"].toBool() );

	connect( displayOffAction, &QAction::triggered, this, [=] (){ displayAction("off"); }  );
	displayOffAction->setCheckable(true);
	displayOffAction->setChecked( shared->server_display["off"].toBool() );
}

void ui_servers_tree::ui(){

	// importa el TreeWidget y agrega las columnas
	this->setColumnCount(10);
    QStringList columns{  "Machine Name", "Status", "Ins.","CPU Usage","CPU Temp", 
    						"RAM Usage","System","IP","VM", "Job Rendered"};
    this->setHeaderLabels(columns);
	this->setSelectionMode( QAbstractItemView::ExtendedSelection ); // multi seleccion
	this->setIndentation(0); // elimina el margen del principio      
	this->setAlternatingRowColors(true); // item con color alternativos

	this->setGeometry(1000,1000,1000,1000);

     // ajusta el largo de las columnas
	this->setColumnWidth(0,150);
	this->setColumnWidth(1,50);
	this->setColumnWidth(2,50);
	this->setColumnWidth(3,177);
	this->setColumnWidth(4,100);
	this->setColumnWidth(5,177);    
	this->setColumnWidth(6,100);    
	//-------------------------------

	this->setContextMenuPolicy(Qt::CustomContextMenu);

	this->setSortingEnabled(true);
	this->sortByColumn(0, Qt::AscendingOrder);

	// Menus de display de servidores
	QMenuBar *menuBar = new QMenuBar();
	menuBar->addMenu("File");
	menuBar->addMenu("Edit");

	QMenu *display = new QMenu( "Filter", monitor );
	menuBar->addMenu( display );

	display->addAction(showAllAction);
	display->addAction(hideAllAction);
	display->addSeparator();

	display->addAction(displayOnAction);
	display->addAction(displayOffAction);
	display->addSeparator();

	display->addAction(displayWindowsAction);
	display->addAction(displayLinuxAction);
	display->addAction(displayMacAction);
	//------------------------------------------

	QWidget *widget = new QWidget();
	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget( menuBar );
	vbox->addWidget( this );

	widget->setLayout(vbox);

	dock->setObjectName("Server");
	dock->setWidget( widget );

	//connect( this, &QTreeWidget::itemDoubleClicked, this, &monitor::serverOptions );

	//----------------------------------------------------------------------------------------------------
}