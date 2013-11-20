#include "stdinThread.h"
#include <QCoreApplication>


void stdinThread::doWork() {

    QString s;
    QFile in;

    qDebug() << "type the letter 'q' and hit return if you want to cancel the download and exit instantly without waiting for the download to finish";

    in.open(stdin, QIODevice::ReadOnly);
    QTextStream ts(&in);
    connect(this, SIGNAL(myquit()), QCoreApplication::instance(), SLOT(quit()));

    while (true) {
        ts >> s;
        qDebug() << "you typed: " << s;

        if (s == "q") {
            emit myquit();
//             QCoreApplication::instance()->quit();
//             QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
            break;
        }
        if (s == "c") {
            emit mycancel();
        }
    }
    qDebug() << "input handling exited";
}
