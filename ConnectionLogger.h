#ifndef CONNECTIONLOGGER_H
#define CONNECTIONLOGGER_H

#include <QObject>
#include <QTcpSocket>
#include <QMap>
#include <Request.h>
#include <Response.h>

namespace BS {

class ConnectionLogger : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionLogger(QObject *parent = 0);
    static ConnectionLogger *sharedLogger();

signals:
    void connected(QString host);
    void disconnected(QString host);

    void incoming_request(QString method, QString host, QString url);
    void sent_response(QString method, QString host, QString briefContent);



public slots:
    void socket_connected(QString addr, quint16 port);
    void socket_disconnected(QString addr, quint16 port);

    void socket_incoming_request(QString method, QString host, QString url);
    void socket_sent_response(QString method, QString host, QString briefContent);

private:
//    QMap < QString, QString > hosts;
};

}

#endif // CONNECTIONLOGGER_H
