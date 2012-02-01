#ifndef CONNECTIONPROCESS_H
#define CONNECTIONPROCESS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <QQueue>
#include <QThread>
#include <QSocketNotifier>
#include <QSharedMemory>
#include <QSemaphore>

class ConnectionProcess : public QThread
{
    Q_OBJECT
public:
    ConnectionProcess(QQueue<QString> * reqQ, QSemaphore *sem);

    //Unix Signal Handler
    static void termSignalHandler(int unused);
    static void hupSignalHandler(int unused);
    static void intSignalHandler(int unused);

public slots:
    void run();
    void terminate();
    void handleSigTerm();
    void handleSigHup();
    void handleSigInt();

private:
    struct sockaddr_in serverAddr;
    int listenFd;

    static int sigTermFd[2];
    static int sigHupFd[2];
    static int sigIntFd[2];

    QSocketNotifier *snterm;
    QSocketNotifier *snhup;
    QSocketNotifier *snint;

    QSharedMemory *shm;
    QQueue<QString> *reqQueue;
    QSemaphore *semEmpty;
};

#endif // CONNECTIONPROCESS_H
