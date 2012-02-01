#include "controllerdaemon.h"
#include <iostream>

using namespace std;

ControllerDaemon::ControllerDaemon(QQueue <QString> *reqQ, QSemaphore *sem)
{
    cout<<"ControllerDaemon Started\n";
    reqQueue = reqQ;
    semEmpty=sem;
}

void ControllerDaemon::run()
{

    //QSharedMemory *shm=new QSharedMemory(*(new QString("12345678")));
    QString reqID;
    //shm->attach();
    while(1)
    {
        //QQueue <QString> *requestQueue=(QQueue <QString> *)shm->data();
        semEmpty->acquire();
        cout<<"Popped\n";
        reqID=reqQueue->front();
        reqQueue->pop_front();
        cout<<endl<<reqID.toAscii().data()<<endl;

        root = new QProcess();
        connect(root,SIGNAL(readyReadStandardOutput()),this,SLOT(stdoutReader()));
        cout<<"Check point 1\n";
        //TODO Do it with Configuration
        QString ROOT_path("/home/swap/root/bin/root");
        QString script_path("/home/swap/mankar/test.C");

        cout<<"Executing\n";
        root->start(ROOT_path,QStringList() << "-l"<<"-b"<<script_path);
        cout<<"pid is: "<<root->pid()<<endl;
        root->waitForReadyRead();
        root->write(".q\n");
        root->waitForBytesWritten();
        root->waitForReadyRead();
        root->terminate();
        delete root;
        /*
        {
            cout<<"Error..!!\n";
            cout<<root->errorString().toAscii().data()<<endl;
        }
        else
        {
            cout<<"Running\n";
        }
        */
        //root->execute(cmd,params);
    }
}
void ControllerDaemon::terminate()
{
    int x;
}

void ControllerDaemon::stdoutReader()
{
    cout<<"CheckPoint 2\n";
    QString stdout(root->readAll());
    cout<<stdout.toAscii().data();
}
