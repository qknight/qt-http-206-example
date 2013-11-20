#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QObject>
#include <QThread>


#include "stdinThread.h"
#include "NetGet.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NetGet ng;


    
    qDebug() << ">> this example will download something defined in NetGet.cpp (if network is working) <<";

    QThread* workerThread1 = new QThread;
    stdinThread stdinthread;

    QObject::connect(workerThread1, SIGNAL(started()), &stdinthread, SLOT(doWork()));
    QObject::connect(workerThread1, SIGNAL(finished()), &stdinthread, SLOT(deleteLater()));

    QObject::connect (&stdinthread, SIGNAL(mycancel()), &ng, SLOT(cancelDownload()));
    QObject::connect (&stdinthread, SIGNAL(myquit()), &a, SLOT(quit()));
    QObject::connect (&a, SIGNAL(aboutToQuit()), &ng, SLOT(cancelDownload()));

    stdinthread.moveToThread(workerThread1);

    workerThread1->start();

    return a.exec();
}
