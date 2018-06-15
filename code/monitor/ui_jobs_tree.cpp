#include "ui_jobs_tree.h"




void ui_jobs_tree::ui(){

	QVBoxLayout *hbox = new QVBoxLayout();
	QLabel *job_tile = new QLabel( "Jobs" );

	job_tile->setAlignment( Qt::AlignHCenter | Qt::AlignHCenter );

	hbox->addWidget( job_tile );
	hbox->addWidget( this );
	widget->setLayout( hbox );


	this->setColumnCount(10);
    QStringList columns { "Job Name", "Priority","Software", "Task Progress", "Status", 
						"Submit Data/Time","Finished Data/Time","Estimated Time","Total Render Time","Comment"};
    this->setHeaderLabels( columns );
	this->setSelectionMode( QAbstractItemView::ExtendedSelection );  // multi seleccion
	this->setAlternatingRowColors( true ); // item con color alternativos
	this->setIndentation(0);  // elimina el margen del principio

	this->setColumnWidth(0,200);   // ajusta el largo de las columnas
	this->setColumnWidth(1,80);
	this->setColumnWidth(2,100);
	this->setColumnWidth(3,200);
	this->setColumnWidth(4,100);
	this->setColumnWidth(5,150);    
	this->setColumnWidth(6,150);    
	this->setColumnWidth(7,150);    
	this->setColumnWidth(8,150);



	this->setContextMenuPolicy( Qt::CustomContextMenu );

	this->setSortingEnabled(true);
	this->sortByColumn( 5, Qt::AscendingOrder );

	//------------------------------------------------------------------------------
}



void ui_jobs_tree::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}


void ui_jobs_tree::dragMoveEvent(QDragMoveEvent *event){
    event->acceptProposedAction();
}

void ui_jobs_tree::dropEvent(QDropEvent *event){
	const QMimeData *mimeData = event->mimeData();
    QList < QUrl > urls = mimeData->urls();
    QString  path = urls[0].toLocalFile();
	uiSubmit->submitDrop( path );
    event->acceptProposedAction();
}


void ui_jobs_tree::mousePressEvent(QMouseEvent *event){
    QTreeView::mousePressEvent(event);

    bool selected = selectionModel()->isSelected(indexAt(event->pos()));
    // si se clickea no en un item, borra las tareas de la lista
    if ( not selected ) taskList->clear();
	//----------------------------------------------------
}
