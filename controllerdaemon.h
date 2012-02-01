#ifndef CONTROLLERDAEMON_H
#define CONTROLLERDAEMON_H
#include <QThread>
#include <QSharedMemory>
#include <QQueue>
#include <QProcess>
#include <QSemaphore>

class ControllerDaemon:public QThread
{
    Q_OBJECT
public:
    ControllerDaemon(QQueue <QString> *reqQ, QSemaphore *sem);
    //void start();

public slots:
    void run();
    void terminate();

    void stdoutReader();
private:
    QProcess *root;
    QQueue <QString> *reqQueue;
    QSemaphore *semEmpty;
};

#endif // CONTROLLERDAEMON_H
