#ifndef BSWEBSERVERCORE_H
#define BSWEBSERVERCORE_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QThreadPool>
#include <ConnectionManager.h>
#include <QProcess>

namespace BS {

class WebServerCore : public QTcpServer
{
    Q_OBJECT
public:
    explicit WebServerCore(QObject *parent = 0);
    int startServer(uint16_t port, const QHostAddress &host = QHostAddress::Any, const QString &homeDirPath = "/");
    void stopServer();
    virtual ~WebServerCore();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:


private slots:

private:
    ConnectionManager *manager;
    QHostAddress host;
    uint16_t port;
    QString homeDirPath;


};

}

#endif // BSWEBSERVERCORE_H

