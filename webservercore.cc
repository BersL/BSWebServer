#include "WebServerCore.h"
#include <QDebug.h>
#include <unistd.h>

namespace BS {

WebServerCore::WebServerCore(QObject *parent) : QTcpServer(parent)
{
    this->manager = new ConnectionManager(this);
//    connect(this, SIGNAL(newConnection()), this, SLOT(handleConnection()));
}

WebServerCore::~WebServerCore() {
    delete this->manager;
}

int WebServerCore::startServer(uint16_t port, const QHostAddress &host, const QString &homeDirPath) {
    if (!this->listen(host, port)) {
        qDebug() << "Unable to start listening on: " << port << "for error: " << this->errorString();
        return -1;
    }
    this->port = port;
    this->host = host;
    this->homeDirPath = homeDirPath;
//    if (chroot(homeDirPath.toUtf8())) {
    chdir(homeDirPath.toUtf8());
//    } else {
//        chdir("/");
//    }
    this->manager->homeDirPath = homeDirPath;

    return 0;
}


void WebServerCore::stopServer() {
    this->close();
    this->manager->destroyAllConnections();
}

void WebServerCore::incomingConnection(qintptr handle) {
    this->manager->incomingConnnection(handle);
}

}
