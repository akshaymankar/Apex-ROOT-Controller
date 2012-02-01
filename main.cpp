#include <QtCore/QCoreApplication>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <QQueue>
#include <QSemaphore>

#include "connectionprocess.h"
#include "controllerdaemon.h"

#include <iostream>

ConnectionProcess *connProc;
ControllerDaemon *rootControl;

using namespace std;
int startup_unix_handlers()
{
    struct sigaction hup, term,int1;

    hup.sa_handler = ConnectionProcess::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0) > 0)
        return 1;

    term.sa_handler = ConnectionProcess::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0) > 0)
        return 2;

    int1.sa_handler = ConnectionProcess::intSignalHandler;
    sigemptyset(&int1.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGINT, &int1, 0) > 0)
        return 3;

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QQueue<QString> *ReqQueue = new QQueue<QString>() ;
    QSemaphore *reqQEmpty = new QSemaphore(0);

    cout<<"Starting Program \n";

    //cout<<"unix Handlers o/p: "<<startup_unix_handlers();
    connProc = new ConnectionProcess(ReqQueue,reqQEmpty);
    rootControl = new ControllerDaemon(ReqQueue,reqQEmpty);

    connProc->start();
    rootControl->start();


    return a.exec();
}
