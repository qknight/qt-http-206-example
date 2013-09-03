#include "stdinThread.h"
#include <QCoreApplication>


void stdinThread::doWork() {

    QString s;
    QFile in;

    qDebug() << "type the letter 'q' and hit return if you want to cancel the download and exit instantly without waiting for the download to finish";

    in.open(stdin, QIODevice::ReadOnly);
    QTextStream ts(&in);

    while (true) {
        ts >> s;
        qDebug() << "you typed: " << s;

        if (s == "q") {
            // FIXME why does this work
            QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
            // but not this (with the upper line commented and the below uncommented?)
//             QObject::connect (&stdinthread, SIGNAL(myquit()), &a, SLOT(quit()));
            break;
        }
        if (s == "c") {
            emit mycancel();
        }
    }
    qDebug() << "input handling exited";
}
