#include "connectionprocess.h"
#include <iostream>
#include <QFile>

using namespace std;
ConnectionProcess::ConnectionProcess(QQueue <QString> *reqQ,QSemaphore *sem)
{
    //save Reference to Request Queue and Semaphore
    reqQueue = reqQ;
    semEmpty =sem;

    //Signal handler
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigHupFd))
            qFatal("Couldn't create HUP socketpair");

    if(::socketpair(AF_UNIX,SOCK_STREAM,0,sigTermFd))
        qFatal("Couldn't create signal handler for SIGTERM.");

    if(::socketpair(AF_UNIX,SOCK_STREAM,0,sigIntFd))
        qFatal("Couldn't create signal handler for SIGTERM.");

    snhup = new QSocketNotifier(sigHupFd[1], QSocketNotifier::Read, this);
    connect(snhup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snterm = new QSocketNotifier(sigTermFd[1],QSocketNotifier::Read,this);
    connect(snterm,SIGNAL(activated(int)),this,SLOT(handleSigTerm()));
    snint = new QSocketNotifier(sigIntFd[1],QSocketNotifier::Read,this);
    connect(snint,SIGNAL(activated(int)),this,SLOT(handleSigInt()));

    QString key;

    cout<<"Started Connection Process !!\n";
}
void ConnectionProcess::run()
{
    //Disable Termination of Thread while creating a socket
    this->setTerminationEnabled(false);

    socklen_t serverAddr_size;

    listenFd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serverAddr,sizeof(serverAddr));

    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;

    //TODO get from Config
    serverAddr.sin_port=htons(10001);
    serverAddr_size = sizeof(serverAddr);

    bind(listenFd,(sockaddr *)&serverAddr,sizeof(serverAddr));

    //TODO get from Config
    listen(listenFd,200);

    //Re-Enable termination of Thread
    this->setTerminationEnabled();

    while(1)
    {
        int connectFd = accept(listenFd,(sockaddr *)&serverAddr,&serverAddr_size);

        //Disable Termination of Thread while handling a request
        this->setTerminationEnabled(false);
        //char request[256];
        int request;
        recv(connectFd,&request,sizeof(request),0);

        //request[strlen(request)-1]='\0';
        cout<<"Req id is: "<<request<<"\n";

        //Write to Queue in shm

        //shm->lock();
        //cout<<"Locked..!!";
        //requQueue =(QQueue <QString> *)shm->data();
        QString reqId = QString::number(request);
        reqQueue->enqueue(reqId);
        semEmpty->release();
        //shm->unlock();

        //Send Ack
        char x[]="Recieved";
        send(connectFd,x,sizeof(x),0);

        close(connectFd);

        //ReEnable Termination
        this->setTerminationEnabled();
    }
}
void ConnectionProcess::terminate()
{
    close(listenFd);
    this->quit();
}
void ConnectionProcess::handleSigTerm()
{
    snterm->setEnabled(false);
    char tmp;
    ::read(sigTermFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    cout<<"SigTerm Handled !!\n";

    snterm->setEnabled(true);
}

void ConnectionProcess::handleSigHup()
{
    snhup->setEnabled(false);
    char tmp;
    ::read(sigHupFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    QFile *fd = new QFile("/home/akshay/work/codes/qt/Apex/ROOT_Controller-build-desktop/akshay");

    fd->write("SigHup Handled !!\n");
    fd->close();
    cout<<"SigHup Handled !!\n";
    snhup->setEnabled(true);
}

void ConnectionProcess::handleSigInt()
{
    snint->setEnabled(false);
    char tmp;
    ::read(sigIntFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    cout<<"SigInt Handled !!\n";
    snint->setEnabled(true);
}

void ConnectionProcess::hupSignalHandler(int unused)
 {
    QFile *fd = new QFile("/home/akshay/work/codes/qt/Apex/ROOT_Controller-build-desktop/akshay");

    fd->write("SigHup Handled !!\n");
    fd->close();

    cout<<"Caught Signal..!!";
    char a = 1;
    ::write(sigHupFd[0], &a, sizeof(a));
 }
void ConnectionProcess::termSignalHandler(int unused)
{
    cout<<"Caught Signal..!!";
    char a = 1;
    ::write(sigTermFd[0], &a, sizeof(a));
}
void ConnectionProcess::intSignalHandler(int unused)
{
    cout<<"Caught Signal..!!";
    char a = 1;
    ::write(sigIntFd[0], &a, sizeof(a));
}
int ConnectionProcess::sigTermFd[2];
int ConnectionProcess::sigHupFd[2];
int ConnectionProcess::sigIntFd[2];
