/* threading.h : CatsFarm 2.0

Esta clase funciona con el QThread, y solo funciona dentro de classes,
se ponen muchos constructores con diferentes cantidades de argumentos,
y se repite la informacion que hay dentro. */

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>

void threading(auto func){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(); });         
	thread->start();
	thread->exit();	
}

void threading(auto func, auto arg1 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(arg1); });         
	thread->start();
	thread->exit();	
}

void threading(auto func, auto arg1, auto arg2 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(arg1, arg2); });         
	thread->start();
	thread->exit();	
}

void threading(auto func, auto arg1, auto arg2, auto arg3 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(arg1, arg2, arg3); });         
	thread->start();
	thread->exit();	
}

void threading(auto func, auto arg1, auto arg2, auto arg3, auto arg4 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(arg1, arg2, arg3, arg4); });         
	thread->start();
	thread->exit();	
}

void threading(auto func, auto arg1, auto arg2, auto arg3, auto arg4, auto arg5 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { func(arg1, arg2, arg3, arg4, arg5); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )(); });         
	thread->start();
	thread->exit();	
}

template < class C >
void  threading(auto (C::*func), C *_class, auto arg1 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2, auto arg3 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2, arg3 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2, auto arg3, auto arg4 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2, arg3, arg4 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2, auto arg3, auto arg4, auto arg5 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2, arg3, arg4, arg5 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2, auto arg3, auto arg4, auto arg5, auto arg6 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2, arg3, arg4, arg5, arg6 ); });         
	thread->start();
	thread->exit();	
}

template < class C >
void threading(auto (C::*func), C *_class, auto arg1, auto arg2, auto arg3, auto arg4, auto arg5, auto arg6, auto arg7 ){
	QThread *thread = new QThread;
	QObject::connect( thread, &QThread::started, [=]() { ( _class->*func )( arg1, arg2, arg3, arg4, arg5, arg6, arg7 ); });         
	thread->start();
	thread->exit();	
}

template < class T >
class qthread_class : public QThread{
public:
    T *_class; 
    void ( T::*func )();
    QTimer *qtimer;

    // Constructor client loop
    qthread_class( void ( T::*_func )(), T *__class ) : QThread(__class) {
        _class = __class;
        func = _func;

        // hilo loop para widget, se inicia por fuera
        qtimer = new QTimer();
        connect ( qtimer, &QTimer::timeout, this, &qthread_class::widget_update );
        qtimer->moveToThread(this);
        //----------------------------------
    } //--------------------------

    void run(){
        qtimer->start(1000);
        exec();

    }

    void widget_update(){
        ( _class->*func )();

    }
};

template < class T >
QThread *qthread( void ( T::*_func )(), T *_class ){

    // inicia thread de update widget
    qthread_class < T >  *_widget = new qthread_class < T >( _func, _class  );     
    _widget->start();
    //-------------------------------------
    return _widget;
}

#endif // MYTHREAD_H