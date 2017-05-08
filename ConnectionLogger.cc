#include "ConnectionLogger.h"
#include <QMutexLocker>
#include <QThread>

static BS::ConnectionLogger *sharedInstance = nullptr;

namespace BS {

ConnectionLogger::ConnectionLogger(QObject *parent) : QObject(parent)
{

}

ConnectionLogger *ConnectionLogger::sharedLogger() {
    static QMutex mutex;
    if (!sharedInstance) {
        QMutexLocker locker(&mutex);
        if (!sharedInstance)
            sharedInstance = new ConnectionLogger;
    }
    return sharedInstance;
}

void ConnectionLogger::socket_connected(QString addr, quint16 port) {
    QString host = QString("%1:%2").arg(addr).arg(port);
    emit connected(host);
}


void ConnectionLogger::socket_disconnected(QString addr, quint16 port) {
    QString host = QString("%1:%2").arg(addr).arg(port);
    emit disconnected(host);
}

void ConnectionLogger::socket_incoming_request(QString method, QString host, QString url) {
    emit incoming_request(method, host, url);
}

void ConnectionLogger::socket_sent_response(QString method, QString host, QString briefContent) {
    emit sent_response(method, host, briefContent);
}


}
