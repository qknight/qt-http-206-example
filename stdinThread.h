#ifndef STDINTHREAD__HH
#define STDINTHREAD__HH

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QFile>

class stdinThread : public QObject {
    Q_OBJECT
public slots:
    void doWork();
signals:
    void myquit();
    void mycancel();
};

#endif
