#include "ui_groups_tree.h"

ui_groups_tree::ui_groups_tree(){
    ui();

}


void ui_groups_tree::ui(){

	string stylesheet = 
		R"(
		QTreeWidget::item:!has-children,
		QTreeWidget::branch::!has-children{
			background-color: rgb(39, 39, 39);
			border-bottom: 1px solid rgb(46, 46, 46);}

		QTreeWidget::item:selected, 
		QTreeWidget::branch:selected,
		QTreeWidget::branch:has-children:closed:selected,
		QTreeWidget::branch:open:selected{ 
			background-color: rgb(30, 45, 60);
			border-bottom: 1px solid rgb(35, 50, 55);
			color: rgb(200, 200, 200);}

		QTreeWidget::item:hover,
		QTreeWidget::branch:hover,
		QTreeWidget::branch:has-children:closed:hover,
		QTreeWidget::branch::!has-children:alternate:hover {
			background-color: rgb(35, 40, 45);}
		)";

	// importa el this y agrega las columnas

	QStringList columns = {"Group Name","Status","name"};
	this->setColumnCount(3);
	this->setHeaderLabels( columns );  // pone el nombre de las columnas
	this->setSelectionMode( QAbstractItemView::ExtendedSelection ); // multi seleccion

	this->setColumnHidden(2,true);

     // ajusta el largo de las columnas
	this->setColumnWidth(0,150);
	this->setColumnWidth(1,70);
	this->setColumnWidth(2,70);
	//----------------------------------

	this->setContextMenuPolicy( Qt::CustomContextMenu );

	this->setSortingEnabled(true);

	this->setStyleSheet( stylesheet.c_str() );

	QWidget *widget = new QWidget();
	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget(this);
	widget->setLayout(vbox);

	dock->setObjectName("Groups");
	dock->setWidget( widget );


	//---------------------------------------------------------------------------------
}
