#ifndef NETGET__HH
#define NETGET__HH

#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QObject>
#include <QUrl>
#include <QVector>


class NetGet : public QObject {
    Q_OBJECT
public:
    NetGet(QObject* parent = 0);
    void printShortcuts();
signals:
    void createRequestSig();
public slots:
    void replyFinished(QNetworkReply* r);
    void cancelDownload();
    void createRequestSlot();
private:
    QNetworkAccessManager* manager;
    QVector<QNetworkReply*> requests;
    bool m_createRequests;
};

#endif
