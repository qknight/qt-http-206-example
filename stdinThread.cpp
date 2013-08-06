#include "stdinThread.h"

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
            emit myquit();
        }
        if (s == "c") {
            emit mycancel();
        }
    }
}
