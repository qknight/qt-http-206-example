#include "NetGet.h"

#include <QFile>
#include <QTextStream>
#include <QString>

NetGet::NetGet(QObject* parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    m_createRequests=true;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    connect(this, SIGNAL(createRequestSig()), this, SLOT(createRequestSlot()));
    printShortcuts();
    emit createRequestSig();
}


NetGet::~NetGet() {
    qDebug() << __FUNCTION__;
    delete manager;
}


void NetGet::printShortcuts() {
    qDebug() << "'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads";
}


void NetGet::createRequestSlot() {
    if (m_createRequests) {
        // this example implements a partial download 206 (http) get and prints the result - 1024 bytes - to a shell using hex digits
        QNetworkRequest rq(QUrl("http://download.qt-project.org/official_releases/online_installers/qt-linux-opensource-1.4.0-x86_64-online.run"));
        long b = qrand()%10000;
        QString s = QString("bytes=1-%1").arg(b);
        rq.setRawHeader("Range", s.toUtf8());
        rq.setRawHeader("Accept:", "*/*");

        qDebug() << "starting download with " << b << " bytes";
        QNetworkReply* rp = manager->get(rq);
        requests.append(rp);

        int ms = qrand() % 5000;
        qDebug() << "sleeping for " << ms << " ms";
        QTimer::singleShot(ms, this, SLOT(createRequestSlot()));
    }
}


void NetGet::cancelDownload() {
    m_createRequests=false;
    qDebug() << "NetGet::cancelDownload() called";
    int i = 0;
    foreach(QNetworkReply* rp, requests) {
        rp->abort();
        delete rp;
        qDebug() << "canceling request " << i++;
    }
    requests.clear();
    printShortcuts();
}


void NetGet::replyFinished(QNetworkReply* r) {
    qDebug() << "replyFinished handler called (this means QNetworkAccessManager was able to do something ;-)";
    if (r->error() == QNetworkReply::NoError) {
        qDebug() << "Content status code: " << r->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "Content type       : " << r->header(QNetworkRequest::ContentTypeHeader);
        qDebug() << "Content size       : " << r->header(QNetworkRequest::ContentLengthHeader);
        QByteArray b = r->readAll();

        qDebug() << "============== Content begin: ==============";
        qDebug() << b.toHex();
        qDebug() << "============== Content end: ==============";
        printShortcuts();
    } else {
        qDebug() << "error on download \n";
    }
}
