#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <ResponseManager.h>
#include <QHostAddress>
#include <RequestManager.h>
#include <IncomingConnection.h>
#include <QList>

namespace BS {

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    QString homeDirPath;

    explicit ConnectionManager(QObject *parent = 0);

    virtual ~ConnectionManager();

    void incomingConnnection(qintptr handle);

    void destroyAllConnections();

signals:
    void socket_connected(QString, quint16);

    void socket_disconnected(QString, quint16);

public slots:
//    void handleReadData();

private:
    ResponseManager *res_manager;

    RequestManager *req_manager;

    QList<IncomingConnection*> establishedConnections;

    void handle(qintptr socketDescriptor);

};

}

#endif // CONNECTIONMANAGER_H
