#ifndef INCOMINGCONNECTION_H
#define INCOMINGCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <RequestManager.h>
#include <ResponseManager.h>

namespace BS {

class IncomingConnection : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;
    RequestManager *req_manager;
    ResponseManager *res_manager;
    QString homeDirPath;

    Response *makeResponseFromRequest(const Request *req);

public:
    explicit IncomingConnection(QTcpSocket *socket, RequestManager *req_manager, ResponseManager *res_manager, QString homeDir, QObject *parent = 0);

    virtual ~IncomingConnection();



signals:

    void incoming_request(QString method, QString host, QString url);

    void sent_response(QString method, QString host, QString briefContent);

public slots:
    void readData();
};

}

#endif // INCOMINGCONNECTION_H
