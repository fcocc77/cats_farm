#include "../hpp/ui_tasks_tree.h"

ui_tasks_tree::ui_tasks_tree()
{
	ui();
}

void ui_tasks_tree::ui()
{

	// importa el this y agrega las columnas

	QStringList columns = {"Task", "Frame Range", "Status", "Server", "Task Time"};
	this->setColumnCount(5);
	this->setHeaderLabels(columns);								  // pone el nombre de las columnas
	this->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
	this->setAlternatingRowColors(true);						  // item con color alternativos
	this->setIndentation(0);									  // elimina el margen del principio

	this->setColumnWidth(0, 70);
	this->setColumnWidth(1, 70);
	this->setColumnWidth(2, 70);
	this->setColumnWidth(3, 140);

	this->setSortingEnabled(true);
	this->sortByColumn(0, Qt::AscendingOrder);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	//connect( this, &QTreeWidget::customContextMenuRequested, this, &monitor::task_popup);

	QWidget *widget = new QWidget();
	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget(this);
	widget->setLayout(vbox);

	dock->setObjectName("task");

	dock->setWidget(widget);

	//---------------------------------------------------------------------------------
}
