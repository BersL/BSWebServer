#include "ConnectionManager.h"
#include <QtConcurrent>
#include <QTcpSocket>
#include <ConnectionLogger.h>
#include <CommonUtils.h>
#include <unistd.h>
#include <limits.h>

#define SOCKET_TIMEOUT 10000

namespace BS {

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent)
{
    this->res_manager = new ResponseManager;
    this->req_manager = new RequestManager;
    connect(this, SIGNAL(socket_connected(QString, quint16)),
            ConnectionLogger::sharedLogger(), SLOT(socket_connected(QString, quint16)));
    connect(this, SIGNAL(socket_disconnected(QString, quint16)),
            ConnectionLogger::sharedLogger(), SLOT(socket_disconnected(QString, quint16)));

}

ConnectionManager::~ConnectionManager() {
    delete this->res_manager;
    delete this->req_manager;
    for (auto it = this->establishedConnections.begin(); it != this->establishedConnections.end(); ++it) {
        delete *it;
    }
}

void ConnectionManager::incomingConnnection(qintptr handle) {
    QFuture<void> res = QtConcurrent::run(this, &ConnectionManager::handle, handle);
}


void ConnectionManager::handle(qintptr socketDescriptor) {
    if (socketDescriptor == 0) return;

    auto socket = new QTcpSocket();
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Set SocketDescriptor error:" << socket->errorString();
        return;
    }
    auto addr = hostAddresstoString(socket->peerAddress());
    auto port = socket->peerPort();
    emit socket_connected(addr, port);

    auto connection = new IncomingConnection(socket, this->req_manager, this->res_manager, this->homeDirPath);
    this->establishedConnections.append(connection);

    QEventLoop loop;
    connect(socket, &QTcpSocket::disconnected, [=](){
        emit socket_disconnected(addr, port);
    });
    connect(socket, SIGNAL(disconnected()), &loop, SLOT(quit()));
    auto timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [&]{
        if (connection) {
            if (this->establishedConnections.count(connection))
                this->establishedConnections.removeOne(connection);
            delete connection;
            connection = nullptr;
        }
    });
    timer->start(SOCKET_TIMEOUT);
    loop.exec();
    timer->stop();

    if (connection) {
        if (this->establishedConnections.count(connection))
            this->establishedConnections.removeOne(connection);
        delete connection;
        connection = nullptr;
    }
}

void ConnectionManager::destroyAllConnections() {
    for (auto it = this->establishedConnections.begin(); it != this->establishedConnections.end(); ++it) {
        delete *it;
        *it = nullptr;
    }
}

}

