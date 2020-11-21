/* threading.hpp : VinaRender 2.0

Esta clase funciona con el QThread, y solo funciona dentro de classes,
se ponen muchos constructores con diferentes cantidades de argumentos,
y se repite la informacion que hay dentro. */

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>

template <class T>
void threading(T func)
{
    QThread *thread = new QThread;
    QObject::connect(thread, &QThread::started, [=]() { func(); });
    thread->start();
    thread->exit();
}

template <class T1, class T2>
void threading(T1(T2::*func), T2 *_class)
{
    QThread *thread = new QThread;
    QObject::connect(thread, &QThread::started, [=]() { (_class->*func)(); });
    thread->start();
    thread->exit();
}

template <class T1, class T2, class T3, class T4, class T5>
void threading(T1(T2::*func), T2 *_class, T3 arg1, T4 arg2, T5 arg3)
{
    QThread *thread = new QThread;
    QObject::connect(thread, &QThread::started, [=]() { (_class->*func)(arg1, arg2, arg3); });
    thread->start();
    thread->exit();
}

template <class T>
class qthread_class : public QThread
{
  public:
    T *_class;
    void (T::*func)();
    QTimer *qtimer;

    // Constructor client loop
    qthread_class(void (T::*_func)(), T *__class) : QThread(__class)
    {
        _class = __class;
        func = _func;

        // hilo loop para widget, se inicia por fuera
        qtimer = new QTimer();
        connect(qtimer, &QTimer::timeout, this, &qthread_class::widget_update);
        qtimer->moveToThread(this);
        //----------------------------------
    } //--------------------------

    void run()
    {
        qtimer->start(1000);
        exec();
    }

    void widget_update()
    {
        (_class->*func)();
    }
};

template <class T>
QThread *qthread(void (T::*_func)(), T *_class)
{

    // inicia thread de update widget
    qthread_class<T> *_widget = new qthread_class<T>(_func, _class);
    _widget->start();
    //-------------------------------------
    return _widget;
}

#endif // MYTHREAD_H