#include "get_manager_info.h"

void get_manager_info::actions(){
	connect( jobsList, &QTreeWidget::itemClicked, this, &get_manager_info::getTask );
}

void get_manager_info::managerRecieve(){

    QString host = fread( "../../etc/manager_host" );

    // actualiza una ves antes del loop.
	QString recv =  tcpClient( host, 7000, jats({ 2,"none" }) );
    managerRecieveUpdate( recv );
    //--------------------------------------
    tcpClient( host, 7000, &get_manager_info::managerRecieveUpdate, this, true );
}

QString get_manager_info::managerRecieveUpdate( QString _recv ){
	QJsonObject recv = jofs( _recv );


	static int timelapse;
	if ( not recv.empty() ){ 
		if ( not shared->stopUpdate ){ 
			shared->jobs = recv["jobs"].toObject(); // para las tasks
			updateJob(  recv["jobs"].toObject() );
			updateServer( recv["servers"].toObject() );
			updateGroup( recv["groups"].toObject() );
		}

		else{ 
			/* va sumando segundos, y si es mayor a 3 seg, se puede actualizar otra vez;
			esto es para que cuando se borre un item, no vuelva a aparecer y desaparecer */
			timelapse++; 
			if ( timelapse > 3 ){  
				shared->stopUpdate = false;
				timelapse = 0;
			}
		}
	}
	
	return jats({2});
}

void get_manager_info::updateJob( QJsonObject recv ){

	// Chekear lista de jobs que hay en la lista
	QStringList job_list;

	struct _job_item{ QString name; QTreeWidgetItem *item; };
	QList <_job_item> job_item;

	for (int i = 0; i < jobsList->topLevelItemCount(); ++i){

		auto item = jobsList->topLevelItem(i); 
		QString name = item->text(0);
		job_list.push_back( name );
		job_item.push_back( { name, item } );

	}
	//-----------------------------

	QStringList job_name_list;

	for ( QJsonValue j : recv ){
		QJsonObject job = j.toObject();
		QString job_name = job["name"].toString();
		QString status = job["status"].toString();
		QString comment = job["comment"].toString();
		QString submit_start = job["submit_start"].toString();
		QString submit_finish = job["submit_finish"].toString();
		QString total_render_time = job["total_render_time"].toString();
		QString estimated_time = job["estimated_time"].toString();
		QString software = job["software"].toString();
		int tasks = job["tasks"].toInt();
		int progres = job["progres"].toInt();

		int porcent = ( progres * 100 ) / tasks;

		// priority text
		QString priority;

		int p = job["priority"].toInt();;

		if ( p == 0 ) priority = "Very High";
		else if ( p == 1 ) priority = "High";
		else if ( p == 2 ) priority = "Normal";
		else if ( p == 3 ) priority = "Low";
		else priority = "Very Low";
		//-------------------------------

		// si el jobs se borro recientemente no se agrega ni actualiza
		bool _delete = false;
		for ( QString d : deleteList ){
			if ( d == job_name ){
				_delete = true; 
			}
		}
		//---------------------------------

		// agrega a la lista para saber que jobs ya no estan
		job_name_list.push_back( job_name );
		//------------------------------------

		if ( not _delete ){

			// Si el nombre no esta en la lista de la interface, agrega el nuevo job.
			if ( not job_list.contains( job_name ) ){

				QTreeWidgetItem *item = new QTreeWidgetItem();

				QProgressBar *taskProgress = new QProgressBar();
				taskProgress->setValue( porcent );
				taskProgress->setMaximumHeight( 17 );
				taskProgress->setObjectName("taskProgress");

				QVBoxLayout *vbox = new QVBoxLayout(); vbox->addWidget(taskProgress);
				vbox->setContentsMargins(0,0,0,0);
				QWidget *widget = new QWidget(); widget->setLayout(vbox);

				//widget->setStyleSheet( "QWidget {background: rgba( 0, 0, 0, 0 );}" );

				item->setText( 0, job_name );
				item->setText( 1, priority );
				item->setText( 2, software );

				item->setText( 4, status );
				item->setText( 5, submit_start );
				item->setText( 6, submit_finish );
				item->setText( 7, estimated_time );

				item->setText( 8, total_render_time );
				item->setText( 9, comment );

				jobsList->addTopLevelItem( item );
				jobsList->setItemWidget( item, 3, widget );

			}	
			//-----------------------------------------------------------------------

			for (int i = 0; i < jobsList->topLevelItemCount(); ++i){

				auto item = jobsList->topLevelItem(i); 
				QString name = item->text(0);
				if ( name == job_name ){
					item->setText( 0, job_name );
					item->setText( 1, priority );
					item->setText( 2, software );

                    QProgressBar *taskProgress = jobsList->itemWidget( item, 3 )->findChild<QProgressBar*>();

					taskProgress->setValue( porcent );
					taskProgress->setFormat(  QString::number( porcent ) + "%   (" + QString::number( progres ) + "/" 
											+ QString::number( tasks ) + ")" );

					item->setText( 4, status );
					item->setText( 5, submit_start );
					item->setText( 6, submit_finish );
					item->setText( 7, estimated_time );
					item->setText( 8, total_render_time );
					item->setText( 9, comment);

					if ( status == "Completed" ){

						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(70, 100, 120);}";
						jobsList->itemWidget( item, 3 )->setStyleSheet( bar_color );

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(80,150,200) );
						}

					}

					if ( status == "Rendering..." ){

						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(50, 120, 70);}";
						jobsList->itemWidget(item,3)->setStyleSheet( bar_color );

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(100,200,100) );
						}

					}

					if ( status == "Failed" ){

						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(200, 0, 0);}";
						jobsList->itemWidget( item, 3 )->setStyleSheet( bar_color );

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(200,0,0) );
						}
					}

					if ( status == "Concatenate" ){

						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(120, 100, 120);}";
						jobsList->itemWidget( item, 3 )->setStyleSheet( bar_color );
						taskProgress->setFormat("....Concat....");

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(180,130,200) );
						}
					}

					if ( status == "Queue" ){
						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(140, 140, 140);}";
						jobsList->itemWidget( item, 3)->setStyleSheet( bar_color );

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(200,200,200) );
						}
					}

					if ( status == "Suspended" ){

						QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(140, 140, 0);}";
						jobsList->itemWidget( item, 3 )->setStyleSheet( bar_color );

						for (int i = 0; i < 10; ++i){
							item->setForeground( i, QColor(200,200,0) );
						}
					}

				}
			}

		}

	}   
	// si el job ya no existe, borra el item

	for ( _job_item ji : job_item  ){
		if ( not job_name_list.contains( ji.name ) ){
			auto root = jobsList->invisibleRootItem();
			root->removeChild( ji.item );

		}

	}
}

void get_manager_info::updateServer( QJsonObject recv ){
	// Chekear lista de server que hay en la lista
	QStringList server_list;
	struct _server_item{ QString name; QTreeWidgetItem *item; };
	QList <_server_item> server_item;

	for (int i = 0; i < serverList->topLevelItemCount(); ++i){
		auto item = serverList->topLevelItem(i); 
		QString name = item->text(0);
		server_list.push_back( name );
		server_item.push_back( { name, item } );
    }
	//--------------------------------------------------

	// crea lista de los server que no estan filtrados
	QStringList displayServer;
	for ( QJsonValue s : recv ){
		QJsonObject server = s.toObject();
		QString server_name = server["name"].toString();
		QString status = server["status"].toString();
		QString system = server["system"].toString();

		bool display = false;
		if ( status == "absent" ){
			if ( shared->server_display["off"].toBool() ){ //OFF
				display = true;
			}
		}

		else{
			if ( shared->server_display["on"].toBool() ){ //ON
				display = true;
			}
		}

		if ( display ){ 
			if ( system == "Windows" ){
				if ( shared->server_display["window"].toBool() ){ //Windows
					displayServer.push_back( server_name );
				}
			}

			if ( system == "Linux" ){
				if ( shared->server_display["linux"].toBool() ){ //Linux
					displayServer.push_back( server_name ); 
				}
			}

			if ( system == "Mac" ){
				if ( shared->server_display["mac"].toBool() ){ //Mac	
					displayServer.push_back( server_name );
				}
			}
	    }

	}
	//------------------------------------------------------
	//*
	QStringList server_name_list;

	for ( QJsonValue s : recv ){ 
        QJsonObject server = s.toObject();
        QString server_name = server["name"].toString();
		int max_instances = server["max_instances"].toInt();
		QString status = server["status"].toString();
		QString host = server["host"].toString();
		QString system = server["system"].toString();
		int cpu = server["cpu"].toInt();
		int ram = server["ram"].toInt();
		int temp = server["temp"].toInt();
		bool vbox_status = server["vbox"].toBool();
		int ram_total = server["ram_total"].toInt();
		float ram_used = server["ram_used"].toDouble();
		int cpu_cores = server["cpu_cores"].toInt();

        QList <inst_struct*> instances;
        for ( QJsonValue i : server["instances"].toArray() ){
        	QJsonArray ins = i.toArray();
        	inst_struct *_ins = new inst_struct;
        	*_ins = { ins[0].toInt(), ins[1].toInt(), ins[2].toBool(), ins[3].toString() };
        	instances.push_back( _ins );
        }

		// crea lista con el nombre de la instancia y el status
		QString instance_job;
		QList <bool> general_status = { 0,0,0,0 };

		if ( status == "absent" ){
			instance_job = "...";
        }
		else{ 
			for (int i = 0; i < max_instances; ++i){
				auto ins = instances[i];
				general_status[ ins->status ] = true;
				instance_job += "   " + ins->job_task;
            }

			if ( general_status[3] ){ status = "failed"; }
			else if ( general_status[2] ){ status = "disabled"; }								
			else if ( general_status[1] ){ status = "active"; }		
			else{ status = "idle"; }
		}
		//--------------------------------------------------------------

		// si el jobs se borro recientemente no se agrega ni actualiza
		bool _delete = false;
		for ( auto d : deleteList ){
			if ( d == server_name ){
				_delete = true;
			}
		}
		//---------------------------------

		if ( not _delete ){
			// a veces cuando lee el archivo de server en el manager, no hay info y queda como None
			// si el server name no igual a None crea el item

			if ( displayServer.contains( server_name ) ){

				// agrega a la lista para saber que server ya no estan
				server_name_list.push_back( server_name );
				//------------------------------------

				// Si el nombre no esta en la lista de la interface, agrega el nuevo job.
				if ( not server_list.contains( server_name ) ){

					QProgressBar *cpuBar = new QProgressBar();
					QVBoxLayout *cpuVbox = new QVBoxLayout(); cpuVbox->addWidget(cpuBar);
					cpuVbox->setContentsMargins(0,0,0,0);
					QWidget *cpuWidget = new QWidget(); cpuWidget->setLayout(cpuVbox);

					QProgressBar *ramBar = new QProgressBar();
					QVBoxLayout *ramVbox = new QVBoxLayout(); ramVbox->addWidget(ramBar);
					ramVbox->setContentsMargins(0,0,0,0);
					QWidget *ramWidget = new QWidget(); ramWidget->setLayout(ramVbox);

					cpuBar->setValue( cpu );
					ramBar->setValue( ram );

					cpuBar->setMaximumHeight(15);
					ramBar->setMaximumHeight(15);

					QTreeWidgetItem *item = new QTreeWidgetItem();

					item->setText( 0, server_name );
					item->setText( 1, status );
					item->setText( 2, QString::number( max_instances ) );
					item->setText( 4, QString::number( temp ) );
					item->setText( 6, system );
					item->setText( 7, host );
					item->setText( 9, instance_job );

					if ( vbox_status ){
						item->setText( 8, "VM is running." );
					}
					else{
						item->setText( 8, "" );
					}
					item->setForeground( 8, QColor(100,200,100) );

					item->setTextAlignment( 4, Qt::AlignHCenter );

					serverList->addTopLevelItem( item );

					serverList->setItemWidget( item, 3, cpuWidget );
					serverList->setItemWidget( item, 5, ramWidget );

					QString bar_color = "QProgressBar::chunk:horizontal {background: rgb(50, 120, 70);}";
					serverList->itemWidget( item, 5 )->setStyleSheet( bar_color );
				}
				//-----------------------------------------------------------------------

				for (int i = 0; i < serverList->topLevelItemCount(); ++i){

					auto item = serverList->topLevelItem(i); 
					QString name = item->text(0);
					if ( name == server_name ){

						if ( ( general_status[1] > 1 ) and ( status == "active" ) ){
							item->setText( 1, QString::number( general_status[1] ) + " " + status );
						}
						else{
							item->setText( 1, status );
						}

						item->setText( 0, server_name );
						item->setText( 2, QString::number( max_instances ) );
						item->setText( 7, host );
						item->setText( 9, instance_job );
						item->setText( 4, QString::number( temp ) + "°C" );

						QProgressBar *cpuBar = serverList->itemWidget( item, 3)->findChild<QProgressBar*>();
						QProgressBar *ramBar = serverList->itemWidget( item, 5)->findChild<QProgressBar*>();

						cpuBar->setFormat( "%p%  (" + QString::number( cpu_cores ) + " Cores)" );
						ramBar->setFormat( "%p%  (" + QString::number( ram_used ) + " GB / " +  QString::number( ram_total ) + " GB)" );

						if ( status == "absent" ){
							item->setForeground( 4, QColor(200,200,200) );
						}
						else if ( temp < 60 ){
							item->setForeground( 4, QColor(100,200,100) );
						}
						else if ( temp < 85 ){
							item->setForeground( 4, QColor(200,200,0) );								
						}
						else{
							item->setForeground( 4, QColor(200,0,0) );
						}

						if ( vbox_status ){
							item->setText( 8, "VM is running." );
						}
						else{
							item->setText( 8, "" ); 
						}

						if ( status == "absent" ){

							item->setForeground( 0,QColor(255,30,30) );
							item->setForeground( 1,QColor(255,30,30) );
							item->setText( 8, "" );

							cpuBar->setValue(0);
							ramBar->setValue(0);
							cpuBar->setFormat( "..." );
							item->setText( 4, "...");
							ramBar->setFormat("...");
                        }

						else{

							cpuBar->setValue( cpu );
							ramBar->setValue( ram );

							if ( general_status[3] ){
								item->setForeground( 0, QColor(200,0,0) );
								item->setForeground( 1, QColor(200,0,0) );
							}
							else if ( general_status[1] ){								
								item->setForeground( 0, QColor(100,200,100) ); 
								item->setForeground( 1, QColor(100,200,100) );
							}
							else if ( general_status[2] ){		
								item->setForeground( 0, QColor(200,200,0) );
								item->setForeground( 1, QColor(200,200,0) );
							}
							else{ 
								item->setForeground( 0, QColor(200,200,200) );						
								item->setForeground( 1, QColor(200,200,200) );
                            }
                        }
                    }
                }
            }
        }
	}

	// si el server ya no existe, o esta oculto borra el item 
	for ( auto si : server_item ){
		if ( not  server_name_list.contains( si.name )){
			auto root = serverList->invisibleRootItem();
			root->removeChild( si.item );

        }
	}
	//--------------------------------------------
	//*/
}

void get_manager_info::updateGroup( QJsonObject recv ){

	if ( not recv.empty() ){
		QStringList group_list;

	    struct _group_item{ QString name; QTreeWidgetItem *item; };
	    vector <_group_item> group_item;
		for (int i = 0; i < groupList->topLevelItemCount(); ++i){
			auto item = groupList->topLevelItem(i); 
			QString name = item->text(2);

			group_list.push_back( name );
			group_item.push_back( { name, item } );
        }


		QStringList group_name_list;
		for ( QJsonValue g : recv ){
			QJsonObject group = g.toObject();
            QString group_name = group["name"].toString();
			bool group_status = group["status"].toBool();
			int totaMachine = group["totaMachine"].toInt();
			int activeMachine = group["activeMachine"].toInt();
			int offMachine = totaMachine - activeMachine;
            QStringList machines;

            for ( QJsonValue server : group["server"].toArray() ){
            	machines.push_back(  server.toArray()[0].toString() );
            }

			// agrega a la lista para saber que grupo ya no estan
			group_name_list.push_back( group_name );
			//------------------------------------

			// Si el nombre no esta en la lista de la interface, agrega el nuevo grupo.
			if ( not group_list.contains( group_name ) ){
				groupActions->groupMake( group_name, totaMachine, activeMachine, offMachine );
            }

			//-----------------------------------------------------------------------
			for (int i = 0; i < groupList->topLevelItemCount(); ++i){

				auto item = groupList->topLevelItem(i); 
				QString name = item->text(2);
				if ( name == group_name ){ 
					groupActions->groupMakeServer( item, machines );

					item->setText( 2, group_name );

					QList <QLabel*> labels =  groupList->itemWidget( item, 1 )->findChildren<QLabel*>();
					QList <QLabel*> name_label =  groupList->itemWidget( item, 0 )->findChildren<QLabel*>();

					labels[1]->setText( QString::number( activeMachine ) );
					labels[3]->setText( QString::number( offMachine ) );
					labels[5]->setText( QString::number( totaMachine ) );

					if ( group_status ){
						name_label[1]->setText( "Group is being used" );
					}
					else{
						name_label[1]->setText( "Group is idle" );
					}

					if ( activeMachine ){
						labels[1]->setStyleSheet( "QLabel{color: rgb(100,200,100);}" );
						name_label[0]->setStyleSheet( "QLabel{color: rgb(220,220,220);  font:10pt;}" );
						if ( group_status ){
							name_label[1]->setStyleSheet( "QLabel{color: rgb(100,200,100);  font:8pt;}" );
						}
						else{
							name_label[1]->setStyleSheet( "QLabel{color: rgb(100,150,220);  font:8pt;}" );
						}
					}
					else{
						labels[1]->setStyleSheet( "QLabel{color: rgb(255,30,30);}" );
						name_label[0]->setStyleSheet( "QLabel{color: rgb(255,30,30);  font:10pt;}" );
						name_label[1]->setStyleSheet( "QLabel{color: rgb(255,30,30);  font:8pt;}" );
                    }

					if ( not offMachine ){
						labels[3]->setStyleSheet( "QLabel{color: rgb(100,200,100);}" );
					}
					else{
						labels[3]->setStyleSheet( "QLabel{color: rgb(255,30,30);}" );
					}

					labels[5]->setStyleSheet( "QLabel{color: rgb(120,150,250);}" );

                }
            }

        }
        awrite("../../log/bugs.txt","4\n");
		// si el grupo ya no existe, borra el item 
		for ( _group_item group : group_item ){				
			if ( not group_name_list.contains( group.name ) ){
				//auto root = groupList.invisibleRootItem();
				//falta esto
				//(group.item.parent() or root).removeChild( group.item );
            }
        }
    }
	else{

		groupList->clear();
    }
}

void get_manager_info::getTask(){

	task_first_add = false;

	if ( getTask_thread->isRunning() )
		getTask_thread->terminate();

	taskList->clear();

	updateTask(); // actualiza una vez antes del hilo
	getTask_thread = qthread( &get_manager_info::updateTask, this );
}

void get_manager_info::updateTask(){
    auto selected = jobsList->selectedItems();
	if ( not selected.empty() ){
		auto _item = selected[0];
		QString job_item_name = _item->text(0);
		QList < QStringList > tasks;

		// encuentra en los paketes recibidos el job seleccionado
		QJsonObject job;
		for ( QJsonValue _job : shared->jobs ){
			if ( _job.toObject()["name"].toString() ==  job_item_name ){ 
				job = _job.toObject();
				break;
			}
		} //-------------------------------------------------

        for ( QJsonValue t : job["task"].toArray() ){
        	QJsonObject task = t.toObject();

			QString frameRange = QString::number( task["first_frame"].toInt() ) + "-" + 
			                    QString::number( task["last_frame"].toInt() );

			tasks.push_back( { task["name"].toString(),  frameRange, task["status"].toString(),
							   task["server"].toString(), task["time"].toString() } 
			);

        }

        auto item_set = [=]( QTreeWidgetItem *item,
        	                    QString taskName, 
        	                    QString frameRange, 
        	                    QString status, 
        	                    QString server, 
        	                    QString tiempo )
        {

			item->setText( 0, taskName );
			item->setText( 1, frameRange );
			item->setText( 2, status );
			item->setText( 3, server );
			item->setText( 4, tiempo );

			if ( status == "active" ){
                for (int i = 0; i < 5; ++i){
                    item->setForeground( i, QColor(100,200,100) );
                }		
            }

			if ( status == "finished" ){
				for (int i = 0; i < 5; ++i){
					item->setForeground( i, QColor(80,150,200) );
				}
			}

			if ( status == "waiting" ){
				for (int i = 0; i < 5; ++i){
					item->setForeground( i, QColor(200,200,200) );
				}
			}

			if ( status == "suspended" ){
				for (int i = 0; i < 5; ++i){
					item->setForeground( i, QColor(200,200,0) );
				}
			}

			if ( status == "failed" ){
				for (int i = 0; i < 5; ++i){
					item->setForeground( i, QColor(200,0,0) );
				}
			}
        };

		if ( not task_first_add ){

			for ( QStringList task : tasks ){

                QString taskName = task[0], 
                frameRange = task[1], 
                status = task[2], 
                server = task[3],
                tiempo = task[4];

				QTreeWidgetItem *item = new QTreeWidgetItem();
                item_set( item, taskName, frameRange, status, server, tiempo );

				taskList->addTopLevelItem(item);

				task_first_add = true;
            }
        }

		else{

			for ( QStringList task : tasks ){
                QString taskName = task[0], 
                frameRange = task[1], 
                status = task[2], 
                server = task[3],
                tiempo = task[4];

                for (int i = 0; i < taskList->topLevelItemCount(); ++i){
					auto item = taskList->topLevelItem(i); 
					QString name = item->text(0);

					if ( name == taskName ){
                        item_set( item, taskName, frameRange, status, server, tiempo );
                        break;
                    }
                }

	        }
	    }

	}

	else{ 
		taskList->clear(); 
	}
}

